package p345

func reverseVowels(s string) string {
	arr := []byte(s)
	var i, j int = 0, len(arr) - 1

	vowels := map[byte]struct{}{
		'a': struct{}{},
		'e': struct{}{},
		'i': struct{}{},
		'o': struct{}{},
		'u': struct{}{},
		'A': struct{}{},
		'E': struct{}{},
		'I': struct{}{},
		'O': struct{}{},
		'U': struct{}{},
	}

	for i < j {
		for i < j {
			if _, ok := vowels[arr[i]]; ok {
				break
			}
			i++
		}

		for i < j {
			if _, ok := vowels[arr[j]]; ok {
				break
			}
			j--
		}

		if i < j {
			arr[i], arr[j] = arr[j], arr[i]
			i++
			j--
		}
	}

	return string(arr)
}
