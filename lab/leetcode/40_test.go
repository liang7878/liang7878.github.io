package leetcode

import "testing"

func Test40(t *testing.T){
	res := combinationSum2([]int{10,1,2,7,6,1,5}, 8)
	t.Logf("%v", res)

	res = combinationSum2([]int{3,1,3,5,1,1}, 8)
	t.Logf("%v", res)
}