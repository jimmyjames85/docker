package main

import (
	"fmt"
	"log"
	"os"
	"os/exec"
)

func ls() {
	// ctx, cancel := context.WithCancel(context.Background())
	// cmd := exec.CommandContext(ctx, "/usr/bin/ls", "-lah")
	cmd := exec.Command("/usr/bin/ls", "-lah")
	cmd.Stderr = os.Stderr
	cmd.Stdout = os.Stdout

	err := cmd.Start()
	if err != nil {
		log.Fatalf("failed to spawn prog: %v", err.Error())
	}

}

func main() {
	fmt.Printf("hello world \n")
	ls()
}
