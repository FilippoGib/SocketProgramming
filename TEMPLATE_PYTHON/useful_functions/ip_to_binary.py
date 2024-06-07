#!/usr/bin/env python3
def ip_to_uint(ip_address: str) -> int:
    octets = ip_address.split('.', 4)
    ip_uint = (int(octets[0]) << 24) + (int(octets[1]) << 16) + (int(octets[2]) << 8) + int(octets[3])
    return ip_uint

def main():
    ip_uint = ip_to_uint('10.10.1.45')
    binary_value = format(ip_uint, '032b')
    print(binary_value)

if __name__ == "__main__":
    main()