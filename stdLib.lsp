(defun std_test (x)
	(if (not (x))
		(println "test " x " not passed")
		)
)

(defun std_transform (l fn)
		(set i 0)
		(while (< i (length l))
			(set result (append result (fn (get l i))))
			(set i (+ i 1))
			)
		(return result)
	)

(defun std_filter (l fn)
		(set i 0)
		(while (< i (length l))
			(if (fn (get l i))
				(set result (append result (get l i)))
			)
			(set i (+ i 1))
		)
		(return result)
	)

(defun std_pow (x p)
	(set i 0)
	(set s x)
	(set p (- p 1))
	(while (< i p)
		(set s (* s x))
		(set i (+ i 1))
		)
	(return s)
	)

(defun std_sqr (x)
	(* x x)
	)

; https://stackoverflow.com/a/39712957
(defun std_sqrt (x)
	(set lo 0)
	(set hi x)
	(set i 0)
	(while (< i 1000)
		(set mid (/ (+ lo hi) 2))
		
		(if (= (std_sqr mid) x)
			(set i 99999) ; break
			)

		(if (> (std_sqr mid) x)
			(set hi mid)
			(set lo mid) ; else
			)

		(set i (+ i 1))
		)
	(return mid)
	)

(defun std_is_even (x)
	(= (% x 2) 0)
	)

(defun std_is_odd (x)
	(= (% x 2) 1)
	)

(defun std_random (prev)
	(+ (* prev 1664525) 1013904223))
	)

