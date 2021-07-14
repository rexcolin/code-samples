package com.company;

import java.io.*;
import java.math.BigInteger;
import java.net.Socket;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.*;
import java.security.cert.CertificateEncodingException;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.cert.Certificate;
import javax.crypto.*;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.PKCS8EncodedKeySpec;


public class Client {



    private static byte[] nonce;
    private static Certificate clientCertificate;
    private static byte[] serverCertificate;
    private static BigInteger privateKey;
    private static BigInteger DHPublic;
    private static byte[] signedDHPublic;
    private static byte[] serverDHPublic;
    private static byte[] signedServerDHPublic;
    private static BigInteger sharedSecret;
    private static byte[] prk;
    private static SecretKeySpec serverEncrypt;
    private static SecretKeySpec clientEncrypt;
    private static SecretKeySpec serverMAC;
    private static SecretKeySpec clientMAC;
    private static IvParameterSpec serverIV;
    private static IvParameterSpec clientIV;

    private static ByteArrayOutputStream messageHistory = new ByteArrayOutputStream();


/****************************************************************
 * MAIN
 * **************************************************************
 */

    public static void main(String[] args) throws IOException, InvalidKeySpecException, CertificateException, NoSuchAlgorithmException, InvalidKeyException, SignatureException, IllegalBlockSizeException, BadPaddingException, InvalidAlgorithmParameterException, NoSuchPaddingException, ClassNotFoundException {

        Socket socket = new Socket( "127.0.0.1", 8080);
        System.out.println("Socket started");

        handshake(socket);
        System.out.println("handshake finished");

        receiveFile(socket, "/Users/rcmitchell/colinMitchell/cs6014/TLSliteDummy/TLSlite/src/bookReceived.txt");
        System.out.println("file received");

        sendACK(socket);
        System.out.println("ACK sent");

    }


/****************************************************************
 * KEY GENERATION
 * **************************************************************
 */

    public static byte[] nonce1() {
        // making random number generator
        SecureRandom random = new SecureRandom();

        // initialize byte array
        byte[] nonce = new byte[32];

        // feed random numbers into byte array
        random.nextBytes(nonce);

//        for (int i = 0; i < nonce.length; i++) {
//            System.out.println(nonce[i]);
//        }
        return nonce;
    }


    public static void setKnownMemberVariables() throws CertificateException, IOException, InvalidKeySpecException, NoSuchAlgorithmException, InvalidKeyException, SignatureException {

        clientCertificate = generateClientCertificate("/Users/rcmitchell/colinMitchell/cs6014/TLSliteDummy/TLSlite/src/CASignedClientCertificate.pem");

        privateKey = getPrivateKey("/Users/rcmitchell/colinMitchell/cs6014/TLSliteDummy/TLSlite/src/clientPrivateKey.der");

        DHPublic = DHPublicKey();

        signedDHPublic = generateSignedDHPublicKey();
    }


    public static void setSessionKeys() throws InvalidKeyException, NoSuchAlgorithmException {

        sharedSecret = getSharedSecret();

        makeSecretKeys();
    }


    public static Certificate generateClientCertificate(String file) throws CertificateException, FileNotFoundException {

        // make certificate factory
        CertificateFactory certificateFactory = CertificateFactory.getInstance("X.509");

        // use it to make certificate
        InputStream certificateInputStream = new FileInputStream(file);
        Certificate certificate = certificateFactory.generateCertificate(certificateInputStream);

        return certificate;
    }


    public static BigInteger DHPublicKey() {
        BigInteger bigPrime = getBigPrimeNumber();
        BigInteger generator = BigInteger.valueOf(2);
        return generator.modPow(privateKey, bigPrime);
    }


    public static BigInteger getPrivateKey(String file) throws IOException {
        // get byte[] from file
        byte[] arr = fileToBytes(file);

        //convert to BigInteger and return
        return new BigInteger(arr);
    }


    public static byte[] generateSignedDHPublicKey() throws NoSuchAlgorithmException, InvalidKeySpecException, InvalidKeyException, SignatureException {
        // convert privateKey and DHpublic to byte array
        byte[] keyInBytes = privateKey.toByteArray();
        byte[] DHpublicInBytes = DHPublic.toByteArray();

        // make Keyfactory, KeySpec to make PrivateKey object
        KeyFactory keyFactory = KeyFactory.getInstance("RSA");
        PKCS8EncodedKeySpec keySpec = new PKCS8EncodedKeySpec(keyInBytes);

        // make PrivateKey object
        PrivateKey PKkey = keyFactory.generatePrivate(keySpec);

        // make Signature and SecureRandom to sign
        Signature signature = Signature.getInstance("SHA256WithRSA");
        SecureRandom secureRandom = new SecureRandom();

        //sign, update with DHpublicInBytes, and return.
        signature.initSign(PKkey, secureRandom);
        signature.update(DHpublicInBytes);
        return signature.sign();

    }


    public static void makeSecretKeys() throws NoSuchAlgorithmException, InvalidKeyException {

        byte[] sharedSecretArr = sharedSecret.toByteArray();

        prk = calculateHMAC(nonce, sharedSecretArr);
        serverEncrypt = new SecretKeySpec(hdkfExpand(prk, "server encrypt"), "AES");
        clientEncrypt = new SecretKeySpec(hdkfExpand(serverEncrypt.getEncoded(), "client encrypt"), "AES");
        serverMAC = new SecretKeySpec(hdkfExpand(clientEncrypt.getEncoded(), "server MAC"), "HmacSHA256");
        clientMAC = new SecretKeySpec(hdkfExpand(serverMAC.getEncoded(), "client MAC"), "HmacSHA256");
        serverIV = new IvParameterSpec(hdkfExpand(clientMAC.getEncoded(), "server IV"));
        clientIV = new IvParameterSpec(hdkfExpand(serverIV.getIV(), "client IV"));

    }


    public static byte[] calculateHMAC(byte[] key, byte[] data) throws InvalidKeyException, NoSuchAlgorithmException {

        //byte[] hmacSha256 = null;
        Mac hmac = Mac.getInstance("HmacSHA256");
        SecretKeySpec secretKeySpec = new SecretKeySpec(key, "HmacSHA256");
        hmac.init(secretKeySpec);
        byte[] hmacSha256 = hmac.doFinal(data);
        return hmacSha256;
    }


    public static byte[] hdkfExpand(byte[] input, String tag) throws NoSuchAlgorithmException, InvalidKeyException {

        byte[] byteTag = tag.getBytes();
        byte[] concatTag = new byte[byteTag.length + 1];
        for (int i = 0; i < byteTag.length; i++) {
            concatTag[i] = byteTag[i];
        }
        concatTag[concatTag.length - 1] = (byte) 1;

        byte[] okm = calculateHMAC(input, concatTag);
        byte[] first16 = new byte[16];
        for (int i = 0; i < 16; i++) {
            first16[i] = okm[i];
        }
        return first16;
    }


    // TODO: figure this out
    public void MAC() {

    }


    // Use 2048-bit prime number from doc
    public static BigInteger getBigPrimeNumber() {

        // TODO: make sure this reads as a hex value
        BigInteger bigPrime = new BigInteger("FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD1" +
                "29024E088A67CC74020BBEA63B139B22514A08798E3404DD" +
                "EF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245" +
                "E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7ED" +
                "EE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3D" +
                "C2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F" +
                "83655D23DCA3AD961C62F356208552BB9ED529077096966D" +
                "670C354E4ABC9804F1746C08CA18217C32905E462E36CE3B" +
                "E39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9" +
                "DE2BCBF6955817183995497CEA956AE515D2261898FA0510" +
                "15728E5A8AACAA68FFFFFFFFFFFFFFFF", 16);

        return bigPrime;
    }


    // first parameter is the other party's public
    public static BigInteger getSharedSecret() {
        BigInteger serverDHBigInt = new BigInteger(serverDHPublic);
        BigInteger bigPrime = getBigPrimeNumber();
        return serverDHBigInt.modPow(privateKey, bigPrime);
    }


/* ***********************************
    Methods to get keys from files
 */

    /**
     * @param file
     * @return byte[]
     * @throws IOException
     */
    public static byte[] fileToBytes(String file) throws IOException {
        // make Path to use in Files method
        Path filePath = Path.of(file);
        // read and return
        return Files.readAllBytes(filePath);
    }


    public String fileToString(String file) throws IOException {
        //make Path to use in Files method
        Path filePath = Path.of(file);

        // read and return
        return Files.readString(filePath);
    }


/****************************************************************
 HANDSHAKE STUFF
 * **************************************************************
 */


    public static void handshake(Socket socket) throws CertificateException, NoSuchAlgorithmException, IOException, SignatureException, InvalidKeyException, InvalidKeySpecException, ClassNotFoundException {

        //System.out.println("starting setKnownMemberVariables()");
        setKnownMemberVariables();

        //System.out.println("starting sendNonce()");
        sendNonce(socket);

        //System.out.println("starting receiveDH()");
        receiveDH(socket);

        //System.out.println("starting sendDH()");
        sendDH(socket);

        //System.out.println("starting setSessionKeys()");
        setSessionKeys();

        //System.out.println("starting receiveMAC()");
        receiveMAC(socket);

        //System.out.println("starting sendMAC()");
        sendMAC(socket);

    }


    /********************
     SEND methods
     ********************/

    public static void sendByteArray(Socket socket, byte[] byteArray) throws IOException {
        ObjectOutputStream outputStream = new ObjectOutputStream(socket.getOutputStream());
        outputStream.writeObject(byteArray);
        outputStream.flush();
    }

    /*
        1:  make nonce
        2:  send nonce
        3:  add nonce to message history
    */
    public static void sendNonce(Socket socket) throws IOException {
        nonce = nonce1();
        sendByteArray(socket, nonce);
        messageHistory.writeBytes(nonce);
    }

    /*
     1:  send message that will contain:
             clientCertificate
             DHPublic
             signedDHPublic
     2:  add message to messageHistory
  */
    public static void sendDH(Socket socket) throws CertificateEncodingException, IOException {
        // convert necessary things to byte arrays and send
        byte[] certificateArr = clientCertificate.getEncoded();
        sendByteArray(socket, certificateArr);
        messageHistory.writeBytes(certificateArr);

        byte[] DHPublicArr = DHPublic.toByteArray();
        sendByteArray(socket, DHPublicArr);
        messageHistory.writeBytes(DHPublicArr);

        sendByteArray(socket, signedDHPublic);
        messageHistory.writeBytes(signedDHPublic);
    }


    /*
        1:  send message that contains messageHistory
        2:  add to messageHistory
    */
    public static void sendMAC(Socket socket) throws NoSuchAlgorithmException, InvalidKeyException, IOException {
        // Add clientMAC to message history
        messageHistory.writeBytes(clientMAC.getEncoded());
        byte[] MACToSend = messageHistory.toByteArray();
        sendByteArray(socket, MACToSend);
        messageHistory.writeBytes(MACToSend);
    }


    // to be used in the file transfer stuff, but it made sense to put it here.
    public static void sendNumberOfMessageChunks(Socket socket, int number) throws IOException {
        ObjectOutputStream outputStream = new ObjectOutputStream(socket.getOutputStream());
        outputStream.writeObject(number);
    }


    /********************
     RECEIVE methods
     ********************/

    public static byte[] receiveByteArray(Socket socket) throws IOException, ClassNotFoundException {
        ObjectInputStream inputStream = new ObjectInputStream(socket.getInputStream());
        byte[] message = (byte[]) inputStream.readObject();
        return message;
    }

    /*
        1:  accept message that will contain:
                    serverCertificate
                    serverDHPublicKey
                    signedServerDHPublic
        2:  set relevant member variables
        3:  add message to messageHistory
    */
    public static void receiveDH(Socket socket) throws IOException, ClassNotFoundException {
        serverCertificate = receiveByteArray(socket);
        messageHistory.writeBytes(serverCertificate);

        serverDHPublic = receiveByteArray(socket);
        messageHistory.writeBytes(serverDHPublic);

        signedServerDHPublic = receiveByteArray(socket);
        messageHistory.writeBytes(signedServerDHPublic);
    }


    /*
     1:  accept message that will contain all previous messages
     2:  verify that it equals messageHistory
     3:  add to message history
  */
    public static void receiveMAC(Socket socket) throws IOException, ClassNotFoundException, NoSuchAlgorithmException, InvalidKeyException {

        // accept MAC from server
        byte[] MACFromServer = receiveByteArray(socket);

        // add serverMAC to our messageHistory to compare
        messageHistory.writeBytes(serverMAC.getEncoded());
        byte[] MACFromClient = messageHistory.toByteArray();

        // verify that it equals messageHistory
        if (MACFromServer.length != MACFromClient.length) {
            // if size doesn't match, print and kill program
            System.out.println("MAC size from server doesn't match client MAC size");
            System.exit(1);
        }
        for (int i = 0; i < MACFromServer.length; i++) {
            if (MACFromServer[i] != MACFromClient[i]) {
                // if data doesn't match, print and kill program
                System.out.println("MAC from server doesn't match client MAC");
                System.exit(1);
            }
        }

        // add to messageHistory
        messageHistory.writeBytes(MACFromServer);
    }


    public static int receiveNumberOfMessageChunks(Socket socket) throws IOException, ClassNotFoundException {
        ObjectInputStream inputStream = new ObjectInputStream(socket.getInputStream());
        return (int) inputStream.readObject();
    }



/****************************************************************
 FILE TRANSFER STUFF
 * **************************************************************
 */


    public static void sendFile(Socket socket, String fileSource) throws IOException, NoSuchPaddingException, InvalidAlgorithmParameterException, NoSuchAlgorithmException, IllegalBlockSizeException, BadPaddingException, InvalidKeyException {

        byte[] wholeFile;
        if (fileSource == "") {
            wholeFile = "ACK".getBytes();
        } else {
            wholeFile = fileToBytes(fileSource);
        }

        int chunksToSend = (int) Math.ceil(wholeFile.length / 1000);

        sendNumberOfMessageChunks(socket, chunksToSend);

        for (int i = 0; i < chunksToSend; i++) {

            int chunkSize;
            if (i != chunksToSend-1) {
                chunkSize = 1000;
            } else {
                chunkSize = wholeFile.length - (i * 1000);
            }
            byte[] chunkToEncrypt = new byte[chunkSize];
            for(int j = 0; j < chunkSize; j++) {
                chunkToEncrypt[j] = wholeFile[j + (i*1000)];
            }

            byte[] HMACToAdd = calculateHMAC(clientEncrypt.getEncoded(), chunkToEncrypt);
            byte[] toEncrypt = concatenate(HMACToAdd, chunkToEncrypt);
            byte[] toSend = encryptData(toEncrypt);

            sendByteArray(socket, toSend);
        }
    }


    public static void receiveFile(Socket socket, String fileDestination) throws IOException, NoSuchPaddingException, InvalidKeyException, NoSuchAlgorithmException, IllegalBlockSizeException, BadPaddingException, InvalidAlgorithmParameterException, ClassNotFoundException {

        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();

        int numberOfMessageChunks = receiveNumberOfMessageChunks(socket);

        for (int i = 0; i < numberOfMessageChunks; i++) {

            byte[] toDecrypt = receiveByteArray(socket);
            byte[] messageWithHMAC = decryptData(toDecrypt);
            byte[] messageNoHMAC = new byte[messageWithHMAC.length - 32];
            System.arraycopy(messageWithHMAC, 32, messageNoHMAC, 0, messageWithHMAC.length - 32);
            //byte[] toWriteOutputStream = calculateHMAC(serverEncrypt.getEncoded(), messageNoHMAC);
            byteArrayOutputStream.writeBytes(messageNoHMAC);
        }

        FileOutputStream fileOutputStream = new FileOutputStream(fileDestination);
        fileOutputStream.write(byteArrayOutputStream.toByteArray());
    }


    public static byte[] encryptData(byte[] toEncrypt) throws NoSuchPaddingException, NoSuchAlgorithmException, InvalidKeyException, InvalidAlgorithmParameterException, BadPaddingException, IllegalBlockSizeException {

        Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
        cipher.init(Cipher.ENCRYPT_MODE, clientEncrypt, clientIV);
        return cipher.doFinal(toEncrypt);
    }


    public static byte[] decryptData(byte[] toDecrypt) throws NoSuchPaddingException, NoSuchAlgorithmException, InvalidAlgorithmParameterException, InvalidKeyException, BadPaddingException, IllegalBlockSizeException {

        Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
        cipher.init(Cipher.DECRYPT_MODE, serverEncrypt, serverIV);
        return cipher.doFinal(toDecrypt);
    }


    public static byte[] concatenate(byte[] a, byte[] b) {
        ByteArrayOutputStream toReturn = new ByteArrayOutputStream();
        toReturn.writeBytes(a);
        toReturn.writeBytes(b);
        return toReturn.toByteArray();
    }


    public static void sendACK(Socket socket) throws NoSuchPaddingException, InvalidKeyException, NoSuchAlgorithmException, IOException, BadPaddingException, IllegalBlockSizeException, InvalidAlgorithmParameterException {

        sendFile(socket, "");

    }

}


