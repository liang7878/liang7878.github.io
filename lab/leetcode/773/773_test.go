package p773

import "testing"

func TestSlidingPuzzle(t *testing.T) {
	t.Logf("result: %d", slidingPuzzle([][]int{
		{1, 2, 3},
		{4, 0, 5},
	}))
}
