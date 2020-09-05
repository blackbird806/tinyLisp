(import "stdLib.lsp")

(println (std_transform (list 1 2 3 4) std_sqr))
(println (std_filter (std_transform (list 1 2 3 4) std_sqr) std_is_even))
(println (std_filter (std_transform (list 1 2 3 4) std_sqr) std_is_odd))

(println (std_filter (std_transform (list 5 8 9 2) std_sqr) std_is_even))
(format "%s %s %s" 5 12 "yikes")

;; (set vec (list 15 12))
;; (println vec)

;; (defun magnitude2 (x)
;; 	(+ (std_sqr (get x 0)) (std_sqr (get x 1)))
;; 	)

;; (defun magnitude (x)
;; 	(std_sqrt (magnitude2 x))
;; 	)

;; (set i 0)
;; (set p 1)
;; (while (< i 10)
;; 	(set p (std_random p))
;; 	(println p)
;; 	(set i (+ i 1))
;; 	)
