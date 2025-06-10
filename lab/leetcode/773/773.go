package p773

import (
	"fmt"
	"strconv"
)

type elem struct {
	board [][]int
	x     int
	y     int
}

func slidingPuzzle(board [][]int) int {
	dir := [][]int{
		{0, 1},
		{0, -1},
		{1, 0},
		{-1, 0},
	}

	var visited map[string]struct{} = make(map[string]struct{})
	var q []elem = make([]elem, 0)
	q[]

	for len(q) != 0 {
		visited["test"] = struct{}{}

	}

	fmt.Printf("%s", convert(board))

	return len(dir)
}

func convert(board [][]int) string {
	res := ""
	for i := 0; i < len(board); i++ {
		for j := 0; j < len(board[i]); j++ {
			res = res + strconv.FormatInt(int64(board[i][j]), 10)
		}
	}
	return res
}
