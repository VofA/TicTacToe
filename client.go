package main

import (
	"bufio"
	"crypto/rsa"
	"fmt"
	"net"
)

// ClientC holds info about connection
type ClientC struct {
	conn                     *net.TCPConn
	address                  net.TCPAddr
	onClientConnectionClosed func(c *ClientC, err error)
	onNewMessage             func(c *ClientC, message string)
	myKey                    *rsa.PrivateKey
	serverKey                *rsa.PublicKey
	buf                      *bufio.Reader
}

// Read client data from channel
func (c *ClientC) Listen() {

	//reader := bufio.NewReader(c.conn)
	for {
		//message, err := reader.ReadByte( /*'\n'*/ )

		// Читаем n байт
		//bytes, err := c.buf.Peek(1024 / 8)
		message, _, err := c.buf.ReadLine()
		if err != nil {
			c.conn.Close()
			c.onClientConnectionClosed(c, err)
			return
		}

		/*for i := 0; i < 1024/8; i++ {
			c.buf.ReadByte()
		}*/

		//msg, err := rsa.DecryptOAEP(sha256.New(), rand.Reader, c.myKey, bytes, nil)

		//log.Println(message)
		c.onNewMessage(c, string(message[:]))
		//c.messages <- string(message[:])
	}
}

// Send text message to client
func (c *ClientC) Send(message string) error {
	//eComm, _ := rsa.EncryptOAEP(sha256.New(), rand.Reader, c.serverKey, []byte(message), nil)

	_, err := c.conn.Write([]byte(message + "\n"))
	return err
}

// SendBytes to client
func (c *ClientC) SendBytes(b []byte) error {
	_, err := c.conn.Write(b)
	return err
}

// Conn returned
func (c *ClientC) Conn() net.Conn {
	return c.conn
}

// Close client connection
func (c *ClientC) Close() error {
	return c.conn.Close()
}

// OnClientConnectionClosed gsgd
func (s *ClientC) OnClientConnectionClosed(callback func(c *ClientC, err error)) {
	s.onClientConnectionClosed = callback
}

// OnNewMessage sfsf
func (s *ClientC) OnNewMessage(callback func(c *ClientC, message string)) {
	s.onNewMessage = callback
}

// Connect dnew tcp server instance
func (s *ClientC) Connect(address net.TCPAddr) {
	con, err := net.DialTCP("tcp4", nil, &address)
	if err != nil {
		panic(err)
	}

	fmt.Println("Connect to", address)
	// log.Println("Connection", con)

	//reply := make([]byte, 1024)
	//con.Read()

	s.conn = con
	s.address = address
	//s.myKey = GenerateKey(1024)
	s.buf = bufio.NewReader(s.conn)

	// log.Println("size=", s.buf.Size)

	go s.Listen()

	//SendKey(*s.conn, s.myKey)

	//s.serverKey = waitprvKserverey(s.buf)
}
