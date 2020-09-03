(import "stdLib.lsp")

;; (defun log_list (x) 
;; 	(set i 0)
;; 	(while ( not (is_null (get x i)))
;; 		(print (get x i) " ")
;; 		(set i (+ i 1))
;; 		(println "")
;; 		)
;; 	)

;; (log_list (list 0 1 2 3 4))
;; (println (square 5))

(defun transform (l fn)
		(set i 0)
		(while (< i (length l))
			(set result (append result (fn (get l i))))
			(set i (+ i 1))
			)
		(return result)
	)

;; (println (transform (list 1 2 3) square))

(defun my_func ()
	(return false)
	)

(std_test my_func)