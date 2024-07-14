
Advanced Encryption Standard (AES) is a very secure encryption algorithm used now a days instead of its predeccesor Data Encryption Standard (DES)
-

The code (in AES_complete file) that I've shared implements the AES (Advanced Encryption Standard) algorithm in C for both encryption and decryption in ECB mode (Electronic Codebook mode). Here's a breakdown of the key components and functions in the code:
-->projectEncyption file is only for encryption
-->projectDecryption file is only decyption of the ciphertext encrypted by projectEncryption file 

Constants and Definitions
-------------------------
AES_block_size: Defines the block size for AES, which is 16 bytes (128 bits).
Nk: Number of 32-bit words in the key. I have used AES-128 i.e which uses 128 bit(16byte) key for encryption and decryption. For AES-128, Nk is 4.
Nr: Number of rounds. For AES-128, Nr is 10.
expandedKeySize: Size of the expanded key in bytes. For AES-128, it's 176 bytes (Nk * (Nr + 1)).

S-boxes and Rcon
----------------
Sbox: Substitution box used for byte substitution during encryption.
rSbox: Reverse substitution box used during decryption.
Rcon: Round constant array used during key expansion.

Key Expansion (keyExpansion)
----------------------------
Function: Expands the original key into a series of round keys.
Input: Original key (key[]), output expanded key (expandedKey[]).
Process: Uses Rijndael's key schedule to expand the key into round keys using byte substitution and rotation operations.

Encryption (ECB_encrypt)
------------------------
Function: Encrypts plaintext using AES in ECB mode.
Input: Plaintext (plaintext[]), expanded key (expandedKey[]), output ciphertext (ciphertext[]), plaintext length (length).
Process: Divides plaintext into 16-byte blocks, performs AES encryption on each block using encryption() function.

Decryption (ECB_decrypt)
------------------------
Function: Decrypts ciphertext using AES in ECB mode.
Input: Ciphertext (ciphertext[]), expanded key (expandedKey[]), output decrypted text (decryptedText[]), ciphertext length (length).
Process: Divides ciphertext into 16-byte blocks, performs AES decryption on each block using decryption() function.

AES Rounds Functions
--------------------
SubBytes (subBytes, invSubBytes): Performs byte substitution using Sbox and rSbox.
ShiftRows (shiftRows, invShiftRows): Performs row-wise circular shifts on the state.
MixColumns (mixColumns, invMixColumns): Mixes columns using a specific matrix multiplication operation.
AddRoundKey (addRoundKey): XORs each byte of the state with the round key.

Helper Functions
----------------
createRoundkey: Creates a round key from the expanded key for a given round.
left_shift, right_shift: Perform left and right circular shifts on rows respectively.
mixColumn, invMixColumn: Mixes columns using specific matrix multiplication for encryption and decryption.

Memory Management
-----------------
Uses malloc to allocate memory dynamically for ciphertext and decrypted text.

Main Function
-------------
Input Handling: Reads plaintext and encryption key from the user.
Execution: Calls keyExpansion, ECB_encrypt, and ECB_decrypt functions to encrypt and decrypt the input text.
Notes
ECB Mode: While ECB mode is straightforward, it has vulnerabilities, especially with repeated plaintext blocks yielding repeated ciphertext blocks.

  Improvements:
  ------------
Padding: Currently, only zero-padding is used. Consider implementing a standard padding scheme like PKCS7.
Input Validation: Add input validation to handle incorrect or unexpected user inputs.
