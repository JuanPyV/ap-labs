package main

import (
	"fmt"
	"os"
	"strings"
)

func main() {
  
  if len(os.Args) > 1 {
    names := os.Args[1:] 
    fullName := strings.Join(names, " ")
  	fmt.Println("Hello "+fullName+", Welcome to the jungle")
  } else {
    fmt.Println("Error message ")
  }
}