(define (navigate num path)
  (define result (find_helper num path))
  (cond
   ((null? result)
     (cons 'not (cons 'found: (cons num '())))
   )
   (else (cons 'found: result))
  )
)

(define (find_helper dest path)
  (cond 
    ((null? path) '()) ; Test if path is empty
    ((eq? dest (car path)) (cons dest '()))  ; Test if node value equals destination
    ((< dest (car path)) ; Take right turn (Left sub-tree)
      (cond
        ((null? (find_helper dest (cadr path))) '())
        (else (cons (car path) (cons 'R (find_helper dest (cadr path)))))
      )
    )
    ((> dest (car path)) ; Take left turn (Right sub-tree)
      (cond
        ((null? (find_helper dest (caddr path))) '())
        (else (cons (car path) (cons 'L (find_helper dest (caddr path)))))
     )
    )
  )
)

(define path '(73 (49 (15 (10 () ()) (20 (17 () ()) (30 () (42 () ())))) (53 () (64 () ()))) (134 (133 (94 (82 (75 () ()) ()) (108 (103 () ()) (110 () ()))) ()) (135 () (136 () (152 (141 () ()) ()))))))

(navigate 42 path)
(navigate 141 path)
(navigate 103 path)
(navigate 81 path)
(navigate 73 path)
