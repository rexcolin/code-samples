package com.company;

import javax.crypto.*;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.io.*;
import java.math.BigInteger;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.*;
import java.security.cert.Certificate;
import java.security.cert.CertificateEncodingException;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.PKCS8EncodedKeySpec;


public class Server {



    private static Certificate serverCertificate;
    private static byte[] clientCertificate;
    private static BigInteger privateKey;
    private static BigInteger DHPublic;
    private static byte[] signedDHPublic;
    private static byte[] clientDHPublic;
    private static byte[] clientSignedDHPublic;
    private static BigInteger sharedSecret;
    private static byte[] clientNonce;
    private static byte[] prk;
    private static SecretKeySpec serverEncrypt;
    private static SecretKeySpec clientEncrypt;
    private static SecretKeySpec serverMAC;
    private static SecretKeySpec clientMAC;
    private static IvParameterSpec serverIV;
    private static IvParameterSpec clientIV;

    private static ByteArrayOutputStream messageHistory = new ByteArrayOutputStream();



/****************************************************************
 * KEY GENERATION
 * **************************************************************
 */

    public static void main(String[] args) throws IOException, InvalidKeySpecException, CertificateException, NoSuchAlgorithmException, InvalidKeyException, SignatureException, IllegalBlockSizeException, BadPaddingException, InvalidAlgorithmParameterException, NoSuchPaddingException, ClassNotFoundException {

        ServerSocket serverSocket = new ServerSocket(8080);
        System.out.println("serverSocket started");

        Socket socket = serverSocket.accept();
        System.out.println("socket accepted");

        handshake(socket);
        System.out.println("handshake finished");

        sendFile(socket, "/Users/rcmitchell/colinMitchell/cs6014/TLSliteDummy/TLSlite/src/mobyDick.txt");
        System.out.println("file sent");

        receiveACK(socket);

        socket.close();

    }


/****************************************************************
 * KEY GENERATION
 * **************************************************************
 */

    public static void setKnownMemberVariables() throws CertificateException, IOException, InvalidKeySpecException, NoSuchAlgorithmException, InvalidKeyException, SignatureException {

        serverCertificate = generateServerCertificate("/Users/rcmitchell/colinMitchell/cs6014/TLSliteDummy/TLSlite/src/CASignedServerCertificate.pem");

        privateKey = getPrivateKey("/Users/rcmitchell/colinMitchell/cs6014/TLSliteDummy/TLSlite/src/serverPrivateKey.der");

        DHPublic = DHPublicKey();

        signedDHPublic = generateSignedDHPublicKey();

    }


    public static void setSessionKeys() throws InvalidKeyException, NoSuchAlgorithmException {
        sharedSecret = getSharedSecret();

        makeSecretKeys();
    }


    public static Certificate generateServerCertificate(String file) throws CertificateException, FileNotFoundException {

        // make certificate factory
        CertificateFactory certificateFactory = CertificateFactory.getInstance("X.509");

        // use it to make certificate
        InputStream certificateInputStream = new FileInputStream(file);
        Certificate certificate = certificateFactory.generateCertificate(certificateInputStream);

        return certificate;
    }

    // private key we're getting from serverPrivateKey.der, the big hex number from the doc, generator is 2
    public static BigInteger DHPublicKey() {
        BigInteger bigPrime = getBigPrimeNumber();
        BigInteger generator = BigInteger.valueOf(2);
        return generator.modPow(privateKey, bigPrime);
    }

    // get from the file
    public static BigInteger getPrivateKey(String file) throws IOException {
        //get byte[] from file
        byte[] arr = fileToBytes(file);

        // convert to a BigInteger and return
        return new BigInteger(arr);
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
        signature.initSign(PKkey,secureRandom);
        signature.update(DHpublicInBytes);
        return signature.sign();

    }

    public static void makeSecretKeys() throws NoSuchAlgorithmException, InvalidKeyException {

        byte[] sharedSecretArr = sharedSecret.toByteArray();

        prk = calculateHMAC(clientNonce, sharedSecretArr);
        serverEncrypt = new SecretKeySpec( hdkfExpand(prk, "server encrypt"), "AES" );
        clientEncrypt = new SecretKeySpec( hdkfExpand(serverEncrypt.getEncoded(), "client encrypt"), "AES" );
        serverMAC = new SecretKeySpec( hdkfExpand(clientEncrypt.getEncoded(), "server MAC"), "HmacSHA256" );
        clientMAC = new SecretKeySpec( hdkfExpand(serverMAC.getEncoded(), "client MAC"), "HmacSHA256" );
        serverIV = new IvParameterSpec( hdkfExpand(clientMAC.getEncoded(), "server IV") );
        clientIV = new IvParameterSpec( hdkfExpand(serverIV.getIV(), "client IV") );

    }

    public static byte[] calculateHMAC(byte[] key, byte[] data) throws InvalidKeyException, NoSuchAlgorithmException {

        byte[] hmacSha256 = null;
        Mac hmac = Mac.getInstance("HmacSHA256");
        SecretKeySpec secretKeySpec = new SecretKeySpec(key, "HmacSHA256");
        hmac.init(secretKeySpec);
        hmacSha256 = hmac.doFinal(data);
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

    public static byte[] fileToBytes(String file) throws IOException {
        //make Path to use in Files method
        Path filePath = Path.of(file);

        //read all the bytes and return
        return Files.readAllBytes(filePath);
    }

    public String fileToString(String file) throws IOException {
        //make Path to use ing Files method
        Path filePath = Path.of(file);

        //read and return
        return Files.readString(filePath);
    }

    // first parameter is the other party's public
    public static BigInteger getSharedSecret() {
        //System.out.println("in getSharedSecret");
        BigInteger serverDHBigInt = new BigInteger(clientDHPublic);
        BigInteger bigPrime = getBigPrimeNumber();
        //System.out.println("about to modpow");
        return serverDHBigInt.modPow(privateKey, bigPrime);
    }



/****************************************************************
 HANDSHAKE STUFF
 * **************************************************************
 */

    public static void handshake(Socket socket) throws CertificateException, NoSuchAlgorithmException, IOException, SignatureException, InvalidKeyException, InvalidKeySpecException, ClassNotFoundException {

        //System.out.println("starting setKnownMemberVariables()");
        setKnownMemberVariables();

        //System.out.println("starting receiveNonce()");
        receiveNonce(socket);

        //System.out.println("starting sendDH()");
        sendDH(socket);

        //System.out.println("starting receiveDH()");
        receiveDH(socket);

        //System.out.println("starting setSessionKeys()");
        setSessionKeys();

        //System.out.println("starting sendMAC()");
        sendMAC(socket);

        //System.out.println("starting receiveMAC()");
        receiveMAC(socket);

    }


    /********************
     SEND methods
     ********************/

    public static void sendByteArray(Socket socket, byte[] byteArray) throws IOException {
        ObjectOutputStream objectOutputStream = new ObjectOutputStream(socket.getOutputStream());
        objectOutputStream.writeObject(byteArray);
        objectOutputStream.flush();

        //put it in message history
        //messageHistory.writeBytes(byteArray);
    }


    /*
        1:  send message that will contain:
                serverCertificate
                DHPublic
                signedDHPublic
        2:  add message to messageHistory
    */
    public static void sendDH(Socket socket) throws CertificateEncodingException, IOException {
        // convert necessary things to byte arrays and send
        byte[] certificateArr = serverCertificate.getEncoded();
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
        2:  add to messageHistory (sendByteArray() does this for us)
    */
    public static void sendMAC(Socket socket) throws NoSuchAlgorithmException, InvalidKeyException, IOException {
        messageHistory.writeBytes(serverMAC.getEncoded());
        byte[] MACToSend = messageHistory.toByteArray();
        sendByteArray(socket, MACToSend);
        messageHistory.writeBytes(MACToSend);
    }

    // to be used in the file transfer stuff, but it made sense to put it here.
    public static void sendNumberOfMessageChunks(Socket socket, int number) throws IOException {
        ObjectOutputStream objectOutputStream = new ObjectOutputStream(socket.getOutputStream());
        objectOutputStream.writeObject(number);
    }



    /********************
     RECEIVE methods
     ********************/

    public static byte[] receiveByteArray(Socket socket) throws IOException, ClassNotFoundException {
        //System.out.println("in receiveByteArray");
        ObjectInputStream inputStream = new ObjectInputStream(socket.getInputStream());
        //System.out.println("about to readAllBytes");
        byte[] message = (byte[]) inputStream.readObject();
        //System.out.println("about to return");
        return message;
    }


    /*
        1:  receive nonce
        2:  set clientNonce
        3:  add nonce to message history
    */
    public static void receiveNonce(Socket socket) throws IOException, ClassNotFoundException {
        //System.out.println("in receiveNonce()");
        clientNonce = receiveByteArray(socket);

        messageHistory.writeBytes(clientNonce);
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
        //System.out.println("in receiveDH");
        clientCertificate = receiveByteArray(socket);
        messageHistory.writeBytes(clientCertificate);

        clientDHPublic = receiveByteArray(socket);
        messageHistory.writeBytes(clientDHPublic);

        clientSignedDHPublic = receiveByteArray(socket);
        messageHistory.writeBytes(clientSignedDHPublic);
    }


    /*
        1:  accept message that will contain all previous messages
        2:  verify that it equals messageHistory
        3:  add to message history
    */
    public static void receiveMAC(Socket socket) throws IOException, ClassNotFoundException, NoSuchAlgorithmException, InvalidKeyException {
        //System.out.println("in receiveMAC");

        // accept MAC from client
        byte[] MACFromClient = receiveByteArray(socket);

        // add clientMAC to our own history
        messageHistory.writeBytes(clientMAC.getEncoded());
        byte[] MACFromServer = messageHistory.toByteArray();


        // verify that it equals messageHistory
        if (MACFromClient.length != MACFromServer.length) {
            // if size doesn't match, print and kill program
            System.out.println("MAC size from client doesn't match server MAC size");
            System.exit(1);
        }
        for (int i = 0; i < MACFromClient.length; i++) {
            if (MACFromClient[i] != MACFromServer[i]) {
                // if data doesn't match, print and kill program
                System.out.println("MAC from client doesn't match server MAC");
                System.exit(1);
            }
        }

        // add to messageHistory
        messageHistory.writeBytes(MACFromClient);
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

        byte[] wholeFile = fileToBytes(fileSource);
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

            byte[] HMACToAdd = calculateHMAC(serverEncrypt.getEncoded(), chunkToEncrypt);
            byte[] toEncrypt = concatenate(HMACToAdd, chunkToEncrypt);
            byte[] toSend = encryptData(toEncrypt);

            sendByteArray(socket, toSend);
        }
    }


    public static byte[] encryptData(byte[] toEncrypt) throws NoSuchPaddingException, NoSuchAlgorithmException, InvalidKeyException, InvalidAlgorithmParameterException, BadPaddingException, IllegalBlockSizeException {

        Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
        cipher.init(Cipher.ENCRYPT_MODE, serverEncrypt, serverIV);
        return cipher.doFinal(toEncrypt);
    }


    public static byte[] concatenate(byte[] a, byte[] b) {
        ByteArrayOutputStream toReturn = new ByteArrayOutputStream();
        toReturn.writeBytes(a);
        toReturn.writeBytes(b);
        return toReturn.toByteArray();
    }

    public static void receiveFile(Socket socket, String fileDestination) throws IOException, NoSuchPaddingException, InvalidKeyException, NoSuchAlgorithmException, IllegalBlockSizeException, BadPaddingException, InvalidAlgorithmParameterException, ClassNotFoundException {

        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();

        int numberOfMessageChunks = receiveNumberOfMessageChunks(socket);

        for (int i = 0; i < numberOfMessageChunks; i++) {

            byte[] toDecrypt = receiveByteArray(socket);
            byte[] messageWithHMAC = decryptData(toDecrypt);
            byte[] messageNoHMAC = new byte[messageWithHMAC.length - 32];
            System.arraycopy(messageWithHMAC, 32, messageNoHMAC, 0, messageWithHMAC.length - 32);
            //byte[] toWriteOutputStream = calculateHMAC(clientEncrypt.getEncoded(), messageNoHMAC);
            byteArrayOutputStream.writeBytes(messageNoHMAC);
        }

        if (fileDestination == "" ) {
            System.out.println("ACK received");
        } else {
            FileOutputStream fileOutputStream = new FileOutputStream(fileDestination);
            fileOutputStream.write(byteArrayOutputStream.toByteArray());
        }
    }


    public static byte[] decryptData(byte[] toDecrypt) throws NoSuchPaddingException, NoSuchAlgorithmException, InvalidAlgorithmParameterException, InvalidKeyException, BadPaddingException, IllegalBlockSizeException {

        Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
        cipher.init(Cipher.DECRYPT_MODE, clientEncrypt, clientIV);
        return cipher.doFinal(toDecrypt);
    }


    public static void receiveACK(Socket socket) throws NoSuchPaddingException, InvalidAlgorithmParameterException, NoSuchAlgorithmException, IOException, BadPaddingException, IllegalBlockSizeException, InvalidKeyException, ClassNotFoundException {

        receiveFile(socket, "");

    }

}
