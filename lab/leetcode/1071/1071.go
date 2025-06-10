package p1071

func gcdOfStrings(str1 string, str2 string) string {
	res := ""

	for i := 0; i < len(str1) && i < len(str2); i++ {
		seq := str1[:i+1]
		if canDivide(str1, seq) && canDivide(str2, seq) && len(seq) > len(res) {
			res = seq
		}
	}

	return res
}

func canDivide(str string, seq string) bool {
	if len(seq) == 0 || len(str) == 0 {
		return false
	}

	if len(str)%len(seq) != 0 {
		return false
	}

	for i := 0; i < len(str); i++ {
		if str[i] != seq[i%len(seq)] {
			return false
		}
	}

	return true
}
