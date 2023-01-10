#lang racket

(provide tree-test)

(require "tree.rkt")
(require "tree-helpers.rkt")

;; (define tree-value tree-root-value)  ;; uncomment this out if you used the name tree-root-value


; first, a convenient trace function that prints output but can
;  all be turned off with the trace-on variable

(define trace-on #t) ; set to #f when not watching it tell you what its doing

(define (trace string)   ; return true for easy use in an "and" below
  (if trace-on
      (begin (display string) (newline) #t)
      #t))  ; don't actually care about the returned value, but Racket wants an 'else'


(define (tree-test)
  (trace (format "Starting tree-test"))
  (let* ( [emptyHedge (list)]
	  [lt1a (tree "leaf 1a"    emptyHedge)]
  	  [lt1b (tree "leaf 1b"    emptyHedge)]
	  [mid1 (tree "mid-node 1" (list lt1a lt1b))]
	  [lt2a (tree "leaf 2a"    emptyHedge)]
  	  [lt2b (tree "leaf 2b"    emptyHedge)]
  	  [lt2c (tree "leaf 2c"    emptyHedge)]
	  [mid2 (tree "mid-node 2" (list lt2a lt2b lt2c))]
	  [top  (tree "top node"   (list mid1 mid2))]
; uncomment once you've written tree-to-code
	  [topAgain  (tree "top node" (list (tree "mid-node 1" (list (leaf "leaf 1a") (leaf "leaf 1b")))
                                            (tree "mid-node 2" (list (leaf "leaf 2a") (leaf "leaf 2b") (leaf "leaf 2c"))))) ]
	  )
    (and
     (trace "Done with tree creation tests, now for tree? predicate.")
     (tree? lt1a)
     (tree? mid2)
     (not (tree? "This is not a tree. Or a pipe. Or an exit. It is a string."))
     (not (tree? (list mid1 mid2)))
     (not (tree? (list "leaf 1a" "leaf 1b")))

     (trace "Done with tree? predicate, now lets look at the result of tree-root-value:")

     (trace (format "(tree-root-value lt1a) is ~a" (tree-root-value lt1a)))
     (trace (format "(tree-root-value mid2) is ~a" (tree-root-value mid2)))
     (trace (format "(tree-root-value top)  is ~a" (tree-root-value top)))

     (trace (format " ... now automatic tests for tree-root-value..."))
     (equal? (tree-root-value lt1a) "leaf 1a")
     (equal? (tree-root-value top) "top node")

     (trace (format " ... now tests for tree=?..."))
     (not (tree=? lt1a lt1b))
     (tree=? lt1a lt1a)
     (tree=? mid2 (tree "mid-node 2" (list (tree "leaf 2a" emptyHedge) (tree "leaf 2b" emptyHedge) (tree "leaf 2c" emptyHedge))))
     (not (tree=? mid2 (tree "mid-n0de 2" (list (tree "leaf 2a" emptyHedge) (tree "leaf 2b" emptyHedge) (tree "leaf 2c" emptyHedge)))))
     (not (tree=? mid2 (tree "mid-node 2" (list (tree "leaf 2a" emptyHedge) (tree "leef 2b" emptyHedge) (tree "leaf 2c" emptyHedge)))))
     (not (tree=? mid2 (tree "mid-node 2" (list (tree "leaf 2a" emptyHedge) (tree "leaf 2c" emptyHedge)))))
     (not (tree=? mid2 (tree "mid-node 2" (list (tree "leaf 2a" emptyHedge) (tree "leaf 2c" emptyHedge) (tree "leaf 2b" emptyHedge)))))

     (trace "Now the rest")

; uncomment the next like after adding child
    (equal? (tree-root-value (tree-child top 0)) "mid-node 1")
; uncomment the next like after adding tree-size (and uncommenting the 'requires' above)
    (equal? (tree-size mid2) 4)
; uncomment the next like after adding tree-height
    (equal? (tree-height-as-stars mid2) "*")
    (equal? (tree-height-as-stars top) "**")
    (trace (format " ... now testing tree=? more ..."))
; uncomment the next like after adding tree-equal?
    (tree=? top top)
    (not (tree=? lt2b lt2c))
    (tree=?      mid2 (tree "mid-node 2" (list (leaf "leaf 2a") (leaf "leaf 2b") (leaf "leaf 2c"))))  ; rebuild mid2
    (not (tree=? mid2 (tree "mid-node 2" (list (leaf "leaf 2a") (leaf "leaf 2c") (leaf "leaf 2b"))))) ; rebuild mid2, diff. order
; uncomment the next like after adding tree-equal-to-code
    (trace (format " ... now testing tree-to-code..."))
    (begin (display (tree-to-code top)) (newline) #t)  ; #t ensures we get "true" for this line, in the "and" test
    (tree=? top topAgain)

; ===========================================================================
;
;  As in the C++ code, you should add some more tests (we will, for grading)
;
; ===========================================================================
     )))

(tree-test)

