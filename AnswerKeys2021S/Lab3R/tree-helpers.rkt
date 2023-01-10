#lang racket

(provide
  tree-height-as-stars
  tree-size
  tree-long-path
  tree-to-code

; KEY:
  tree-map          ; takes a T1->T2 function and Tree<T1>, returns Tree<T2>
  tree-fold         ; takes a function T1xList<T2>->T2 and Tree<T1>, returns T2

; At least one reduce or fold here; give the name, and then describe the types here or below
)

(require "tree.rkt")

;; KEY is below

;first two functions are mutual recursions without higher order list functions
(define (tree-size-brd t)
  (letrec ([hedge-size (λ (hedge) (if (empty? hedge) 0
                                                     (+ (tree-size-brd (car hedge)) (hedge-size (cdr hedge)))))])
            (+ 1 (hedge-size (tree-children t)))))

(define (tree-height-brd t)
  (letrec ([hedge-height (λ (hedge) (if (empty? hedge) 0
                                                       (max (tree-height-brd (car hedge)) (hedge-height (cdr hedge)))))])
            (if (leaf? t) 0
                         (+ 1 (hedge-height (tree-children t))))))

;the next two use higher order list functions - fold down the tree, map across the hedge
(define (tree-height1 a-tree)
  (if (leaf? a-tree) 0
      (+ 1 (foldr max 0 (map tree-height1 (tree-children a-tree))))))

(define (tree-size1 t) 
  (+ 1 (foldr +   0 (map tree-size2   (tree-children t)))))

(define (tree-height-as-stars t)
  (letrec ([starman (λ (n) (if (<= n 1) "*" (string-append "*" (starman (- n 1)))))])
    (starman (tree-height t))))

; string-append* takes a single list of strings and appends them.
; see https://docs.racket-lang.org/reference/strings.html#%28def._%28%28lib._racket%2Fstring..rkt%29._string-append%2A%29%29
(define (hedge-to-code a-hedge)
  (string-append "(list"
                 (string-append* (map (λ (t) (string-append " " (tree-to-code1 t)))
                                      a-hedge))
                 ")"))

(define (tree-to-code1 a-tree)
  (if (leaf? a-tree)  ; could handle this with the general case and size-zero hedge, but yuck.
      (string-append "(leaf \"" (tree-root-value a-tree) "\")")
      (string-append "(tree \"" (tree-root-value a-tree) "\" "
                     (hedge-to-code (tree-children a-tree))
                     ")")))

; would like a "map" that wants a binary function and a pair of lists ...
;  looks like Racket's "map" already does that :-)
; (map + (list 1 2 3) (list 10 20 30))
;(define (tree=? tree1 tree2)
;  (and (string=? (tree-root-value   tree1) (tree-root-value   tree2))
;       (=        (tree-num-children tree1) (tree-num-children tree2))
;       (foldl (λ (p q) (and p q)) #t (map tree=? (tree-children tree1) (tree-children tree2)))))

; this one has a useful racket function that replaces the "foldl" above
;https://docs.racket-lang.org/reference/pairs.html#%28def._%28%28lib._racket%2Fprivate%2Fmap..rkt%29._andmap%29%29
;(define (alt_tree=? tree1 tree2)
;  (and (string=? (tree-root-value   tree1) (tree-root-value   tree2))
;       (=        (tree-num-children tree1) (tree-num-children tree2))
;        (andmap tree=? (tree-children tree1) (tree-children tree2))))

(define (tree-map f t)	; takes a T1->T2 function and Tree<T1>, returns Tree<T2>
  (tree (f (tree-root-value t)) (map f (tree-children t))))


(define (tree-fold f t)	; takes a function T1xList<T2>->T2 and Tree<T1>, returns T2
  (f (tree-root-value t) (map (λ (t) (tree-fold f t)) (tree-children t))))

;this height function uses higher order tree fold, not just list fold
(define (tree-height2 t) (- (tree-fold (λ (s sizes) (+ 1 (foldr max 0 sizes))) t) 1))

;this size function uses higher order tree fold, not just list fold
(define (tree-size2   t)  ; use tree-fold for recursion, list-fold across siblings
  (tree-fold (λ (s sizes) (foldr + 1 sizes)) t))

; (only) This one requires that we have a tree of _strings_; others are polymorphic
(define (tree-to-code2 t)
  (tree-fold (λ (string-val tree-codes)
               (string-append "(tree "
                              "\"" string-val "\" (list"
			      (string-append* (map (λ (tc) (string-append " " tc)) tree-codes))
			      "))"))
	     t))


(define tree-height  tree-height2)
(define tree-size    tree-size2)
(define tree-to-code tree-to-code2)

(define/contract (longest-string-and-its-length strings)
  (-> (λ (the-strings) (andmap string? the-strings))
      (λ (a-pair) (and (string? (car a-pair)) (integer? (cdr a-pair)) (= (string-length (car a-pair)) (cdr a-pair)))))
  (foldr (λ (str len so-far) (if (> len (cdr so-far)) (cons str len) so-far))
         (cons "" 0)
         strings
         (map string-length strings)))

(define/contract (tree-long-path t)
  (-> tree? string?)
  (tree-fold
   (λ (rootval path-choices)
     (string-append rootval (first (longest-string-and-its-length (path-choices)))))
   t)) 
  
