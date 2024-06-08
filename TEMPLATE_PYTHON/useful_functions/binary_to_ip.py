def int_to_ip(ip_int):
    return '.'.join(map(str, [
        (ip_int >> 24) & 0xFF,
        (ip_int >> 16) & 0xFF,
        (ip_int >> 8) & 0xFF,
        ip_int & 0xFF
    ]))

def main():
    print(int_to_ip(3232235779))

if __name__ == "__main__":
    main()