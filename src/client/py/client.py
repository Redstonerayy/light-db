import socket

HOST = "127.0.0.1"
PORT = 4466

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))

    s.sendall(b"5 23;")
    data = s.recv(1024)
    print(f"Received {data!r}")

    s.sendall(b"6 23;")
    data = s.recv(1024)
    print(f"Received {data!r}")

    s.sendall(b"7 23;")
    data = s.recv(1024)
    print(f"Received {data!r}")

    s.sendall(b"4 23;")
    data = s.recv(1024)
    print(f"Received {data!r}")
