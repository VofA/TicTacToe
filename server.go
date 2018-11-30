package main

import (
	"bufio"
	"fmt"
	"net"
)

const ()

// Client holds info about connection
type Client struct {
	conn   net.Conn
	Server *Server
	//myKey     *rsa.PrivateKey
	//clientKey *rsa.PublicKey
	buf *bufio.Reader
}

// TCP Server
type Server struct {
	address                  string
	onNewClientCallback      func(c *Client)
	onClientConnectionClosed func(c *Client, err error)
	onNewMessage             func(c *Client, message string)
}

// Read client data from channel
func (c *Client) Listen() {
	//reader := bufio.NewReader(c.conn)
	for {
		//message, err := reader.ReadByte( /*'\n'*/ )
		message, _, err := c.buf.ReadLine()

		// Читаем n байт
		//bytes, err := c.buf.Peek(1024 / 8)
		if err != nil {
			c.conn.Close()
			c.Server.onClientConnectionClosed(c, err)
			return
		}

		/*for i := 0; i < 1024/8; i++ {
			c.buf.ReadByte()
		}*/

		//msg, err := rsa.DecryptOAEP(sha256.New(), rand.Reader, c.myKey, bytes, nil)

		c.Server.onNewMessage(c, string(message[:]))
	}
}

// Send text message to client
func (c *Client) Send(message string) error {
	//eComm, _ := rsa.EncryptOAEP(sha256.New(), rand.Reader, c.clientKey, []byte(message), nil)

	_, err := c.conn.Write([]byte(message + "\n"))
	return err
}

// SendBytes to client
func (c *Client) SendBytes(b []byte) error {
	_, err := c.conn.Write(b)
	return err
}

// Conn returned
func (c *Client) Conn() net.Conn {
	return c.conn
}

// Close client connection
func (c *Client) Close() error {
	return c.conn.Close()
}

// Called right after Server starts listening new client
func (s *Server) OnNewClient(callback func(c *Client)) {
	s.onNewClientCallback = callback
}

// Called right after connection closed
func (s *Server) OnClientConnectionClosed(callback func(c *Client, err error)) {
	s.onClientConnectionClosed = callback
}

// Called when Client receives new message
func (s *Server) OnNewMessage(callback func(c *Client, message string)) {
	s.onNewMessage = callback
}

// Start network Server
func (s *Server) Listen() {
	listener, err := net.Listen("tcp4", s.address)
	if err != nil {
		panic(err)
		//log.Fatal("Error starting TCP Server.")
	}
	defer listener.Close()

	for {
		conn, _ := listener.Accept()
		client := &Client{
			conn:   conn,
			Server: s,
			//myKey:  GenerateKey(1024),
			buf: bufio.NewReader(conn),
		}

		//go client.Listen()
		go client.Listen()

		//client.clientKey = waitprvKServerey(client.buf)

		//SendKey(client.conn, client.myKey)

		s.onNewClientCallback(client)
	}
}

// New tcp Server instance
func New(address string) *Server {
	fmt.Println("Creating Server with address", address)
	Server := &Server{
		address: address,
	}

	Server.OnNewClient(func(c *Client) {})
	Server.OnNewMessage(func(c *Client, message string) {})
	Server.OnClientConnectionClosed(func(c *Client, err error) {})

	return Server
}
