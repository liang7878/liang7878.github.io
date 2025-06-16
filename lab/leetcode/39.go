package leetcode

func combinationSum(candidates []int, target int) [][]int {
    res := make([][]int, 0)
    tmp := make([]int, 0)
    helper(candidates, target, 0, tmp, &res)
    return res
}

func helper(candidates []int, target int, start int, tmp []int, res *[][]int) {
    if target == 0 {
        *res = append(*res, tmp)
        return
    }

    for i := start; i < len(candidates); i++ {
        if candidates[i] > target {
            continue
        } 
        tmp = append(tmp, candidates[i])
        helper(candidates, target - candidates[i], i, append([]int{}, tmp...), res)
        tmp = tmp[:len(tmp)-1]
    }
}