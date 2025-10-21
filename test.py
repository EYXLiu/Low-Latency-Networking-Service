import socket

s = socket.socket()
s.connect(('localhost', 5555))

payload = b"hi"
length = len(payload)
msg = length.to_bytes(2, 'big') + bytes([1]) + (1234).to_bytes(4, 'big') + payload

s.send(msg)