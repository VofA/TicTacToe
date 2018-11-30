package main

import (
	"fmt"
	"log"
	"net"
	"strconv"
)

const (
	PLAYER_X = 1
	PLAYER_O = 2
)

var (
	grid = [3][3]int{{3, 4, 5}, {6, 7, 8}, {9, 10, 11}}

	isServer = false
	me       = PLAYER_X

	server  *Server
	sClient *Client

	client ClientC

	messages chan string
)

func main() {
	log.SetFlags(log.LstdFlags | log.Lshortfile)
	messages = make(chan string)

	fmt.Print("Create new server (if not connect): ")
	fmt.Scan(&isServer)

	if isServer {
		server = New("0.0.0.0:1338")

		server.OnNewMessage(func(c *Client, message string) {
			// updateChan(message)
			// log.Println("Recieved: ", message)
			messages <- message
		})
		server.OnNewClient(func(c *Client) {
			sClient = c
			go game()
		})
		go server.Listen()
	} else {
		/*fmt.Print("Enter ip: ")
		var ipS string
		fmt.Scan(&ipS)

		ip := net.ParseIP(ipS)
		log.Println(ip)*/
		me = PLAYER_O
		client.OnNewMessage(func(c *ClientC, message string) {
			// updateChan(message)
			// log.Println("Recieved: ", message)
			messages <- message
		})
		//client.SetChan(messages)
		client.Connect(net.TCPAddr{IP: net.IPv4(0, 0, 0, 0), Port: 1338})

		go game()
	}

	for {
	}
}

func game() {
	isX := true
	myHod := isServer

	for i := 0; i < 9; i++ {
		var x, y int
		// log.Print("if ", isX, me, PLAYER_X, isX && me == PLAYER_X)
		// if isX && me == PLAYER_X {
		if myHod {
			// log.Println("my hod")
			fmt.Print("Enter x, y coordinates: ")

			fmt.Scan(&x, &y)
			x--
			y--
			if x > 2 || x < 0 || y > 2 || y < 0 || grid[y][x] == 1 || grid[y][x] == 2 {
				i--
				fmt.Println("!!!incorrect!!!")
				continue
			}

			result := strconv.Itoa(x) + strconv.Itoa(y)

			if isServer {
				sClient.Send(result)
			} else {
				client.Send(result)
			}
		} else {
			fmt.Println("Wait hod")
			response := <-messages
			// log.Println("response", response)

			x, _ = strconv.Atoi(response[0:1])
			y, _ = strconv.Atoi(response[1:])

			// log.Println(response[0:1], response[1:])
			// log.Println(x, y)
		}

		if isX {
			// if myHod {
			grid[y][x] = PLAYER_X
		} else {
			grid[y][x] = PLAYER_O
		}

		isX = !isX
		myHod = !myHod

		fmt.Println("Grid: ")
		drawGrid()

		if checkWin() {
			if grid[y][x] == 1 {
				fmt.Println("X-Winner!")
			} else {
				fmt.Println("O-Winner!")
			}
			return
		}
	}
}

func drawGrid() {
	for y := 0; y < 3; y++ {
		for x := 0; x < 3; x++ {
			if grid[x][y] == PLAYER_X {
				fmt.Print("X")
			} else if grid[x][y] == PLAYER_O {
				fmt.Print("O")
			} else {
				fmt.Print(" ")
			}
			fmt.Print(" | ")
		}
		fmt.Println()
	}
}

func checkWin() (status bool) {
	return (
	// Horisontal lines
	(grid[0][0] == grid[0][1] && grid[0][1] == grid[0][2]) ||
		(grid[1][0] == grid[1][1] && grid[1][1] == grid[1][2]) ||
		(grid[2][0] == grid[2][1] && grid[2][1] == grid[2][2]) ||

		// Vertical lines
		(grid[0][0] == grid[1][0] && grid[1][0] == grid[2][0]) ||
		(grid[0][1] == grid[1][1] && grid[1][1] == grid[2][1]) ||
		(grid[0][2] == grid[1][2] && grid[1][2] == grid[2][2]) ||

		// X
		(grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2]) ||
		(grid[2][0] == grid[1][1] && grid[1][1] == grid[0][2]))
}
