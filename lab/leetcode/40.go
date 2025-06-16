package leetcode

import "sort"

func combinationSum2(candidates []int, target int) [][]int {
    res := make([][]int,0)
    tmp := make([]int, 0)

    sort.Ints(candidates)
    helper40(candidates, target, 0, tmp, &res)
    return res
}

func helper40(candidates []int, target int, index int, tmp []int, res *[][]int) {
    if target < 0 {
        return
    }

    if target == 0 {
        *res = append(*res, tmp)
        return
    }

    for i:=index; i < len(candidates); i++ {
		if i > index && candidates[i] == candidates[i-1] {
			continue
		}
		tmp = append(tmp, candidates[i])
        helper40(candidates, target-candidates[i], i + 1, append([]int{}, tmp...), res)
		tmp = tmp[:len(tmp)-1]
    }
}