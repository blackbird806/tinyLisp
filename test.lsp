(import "stdlib.lsp")

(println (std_transform (list 1 2 3 4) std_sqr))
(println (std_filter (std_transform (list 1 2 3 4) std_sqr) std_is_even))
(println (std_filter (std_transform (list 1 2 3 4) std_sqr) std_is_odd))

(println (std_filter (std_transform (list 5 8.074 9.545 2) std_sqr) std_is_even))
(println (std_filter (std_transform (list 5 8 9 2 2 3 4 4 8 7 89 54 5 654 654 564213 84 545 4 21 151 51 51 84 54 54 78 75 45 65 6 5) std_sqr) std_is_even))

(set i 0)
(while (< i 100)
	(println (std_filter (std_transform (list 5 8 9 2 2 3 4 4 8 7 89 54 5 654 654 564213 84 545 4 21 151 51 51 84 54 54 78 75 45 65 6 5) std_sqr) std_is_even))
	(set i (+ i 1))
	)