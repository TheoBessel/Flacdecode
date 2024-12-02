# Flacdecode

This little project is a decoder for FLAC Files written in C++ during my first year of engineering school for fun.
It only implements the decoding of binary FLAC files without focusing on the compressing part which was, not so simple as long as the FLAC encoding is using both big and little endianness depending of the type of header.

For this project I only used the FLAC documentation (available [here](https://xiph.org/flac/documentation.html)) which was an extra challenge.

Below there is a little example of the decoding of FLAC metadata header :

<img width="450" alt="Capture_decran_2023-10-16_a_13 02 13" src="https://github.com/user-attachments/assets/360e71f6-a1c8-4e36-a478-290c19e8e893">
