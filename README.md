
# CUDA_AES256_Implementation

This project aims to create an implementation of **Advanced Encryption Standard 256** or **AES256** for NVIDIA CUDA hardware. The goal of this implementation is to speed  up the encryption and decryption process compared to CPU processing by using the parallel nature of GPU processing.

At the end of the project, benchmarks compared to CPU processing will be performed to allow insights for further optimization of this implementation.
## Features

- Encrypt and decrypt files using AES256
- Simultaneous processing of files
- Customability of encryption by allowing user-defined salts




## Roadmap

- Implement a system where data can be immediately processed from data extractede from a file

- Implement kernel functions that perform the steps of AES Encryption

- Use the openssl library to securely generate keys for Encryption
