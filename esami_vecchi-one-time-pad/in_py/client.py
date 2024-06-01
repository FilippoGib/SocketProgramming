#!/usr/bin/env python3

import socket
import sys

HOST = '127.0.0.1'
PORT = 8080
FORMAT = 'utf-8'

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST,PORT))
    one = '001'
    two = '002'
    three = '003'
    four = '004'

    with open('001.txt', 'r') as file:
        file_content = file.read()

    one += file_content

    with open('002.txt', 'r') as file:
        file_content = file.read()

    two += file_content

    with open('003.txt', 'r') as file:
        file_content = file.read()

    three += file_content

    with open('004.txt', 'r') as file:
        file_content = file.read()

    four += file_content

    otp = ''.join([one.replace('\n', ''), 
                  two.replace('\n', ''), 
                  three.replace('\n', ''), 
                  four.replace('\n', '')]) #this is my reference encoder string 

    print("Debugging: otp = " + otp)

    input = s.recv(1024).decode(FORMAT, errors = 'strict')
    input = input.upper()

    print("Debugging: input = " + input)
    output = ""

    i = 0
    j = 0

    while i < len(input):

        if(j >= len(otp)): #j itera otp ricominciando se necessario
            j = 0

        if(otp[j].isdigit()): #aggiungo ad output il numero dell'otp corrente
            output += otp[j]
            j += 1
            continue

        if(input[i].isalpha() & otp[j].isalpha()):
            print("Debugging: input[i] = " + input[i] + " otp[j] = " + otp[j])
            print("")
            encoded_char = chr(((ord(input[i]) - ord('A') + ord(otp[j]) - ord('A')) % 26) + ord('A'))

        else:
            encoded_char = "-"
            j -= 1
        
        output += encoded_char

        i += 1
        j += 1
    
    output += "."
    s.sendall(output.encode(FORMAT, errors = "strict"))
    s.close

print("client terminato\n")