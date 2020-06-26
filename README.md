# Playing around with gRPC and TLS

# Works
* `make certs-rsa`
* `make server && ./server`
* `make client && ./client`

# Doesn't work
* `make certs-ec`
* `make server && ./server`
* `make client && ./client`

```
E0626 09:14:49.276613726   48012 ssl_transport_security.cc:1379] Handshake failed with fatal error SSL_ERROR_SSL: error:1417A0C1:SSL routines:tls_post_process_client_hello:no shared cipher.
```

# Works
* `make certs-ec`
* `openssl s_server -cert server.cert -key server.key -accept 50051 -rev`
* `openssl s_client -connect localhost:50051`
