(import "stdLib.lsp")

(println (std_transform (list 1 2 3 4) std_sqr))
(println (std_filter (std_transform (list 1 2 3 4) std_sqr) std_is_even))
(println (std_filter (std_transform (list 1 2 3 4) std_sqr) std_is_odd))