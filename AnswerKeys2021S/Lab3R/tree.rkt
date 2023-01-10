#lang racket

(provide
  ; *** constructors: provide a base-case, either 'leaf' or 'empty-tree', _and_ recursive case, tree:
  leaf              ; takes one parameter a string, returns a leaf containing that value
;;  empty-tree        ; takes no parameters, returns an empty tree
  tree              ; build a tree from a root value (a string) and a list of subtrees

  ; *** predicates: provide tree?, hedge?, and one or more predicates to detect useful base cases, and a tree equality test
  tree?             ; predicate to return true if and only if its argument is a valid tree
  leaf?             ; predicate to return true if and only if its argument is a _leaf_
  hedge?            ; predicate to return true iff given a list of trees
;;  empty-tree?       ; predicate to return true if and only if its argument is an _empty_tree_
  tree=?
  
  ; *** accessors: provide at least these four
  tree-root-value   ; takes one non-empty tree, returns the root value
  tree-children     ; takes one non-empty tree, returns the list of children
  tree-n-children   ; takes one non-empty tree, returns the _number_ of children
  tree-child        ; takes one non-empty tree and an integer i, returns child tree #i

;  functions that make use of the above to provide additional functionality belong in tree-helpers.rkt
)

;; KEY below, and in having made choice of leaf/leaf? above rather than tree-empty?

(define (tree? maybe-a-tree)   ; pretty well armoured, I think ... shouldn't crash for anything
  (and
   (list?   maybe-a-tree)
   (=       (length maybe-a-tree) 3)
   (eq?     (first  maybe-a-tree) tree)  ; check type
   (string? (second maybe-a-tree))
   (hedge?  (third  maybe-a-tree))))

(define (hedge? maybe-a-hedge)
  (and
   (list?   maybe-a-hedge)
   (andmap  tree? maybe-a-hedge)))  ;; is each thing in "hedge" a tree?


; don't know why I couldn't get this to work with define/contract...
(define (tree value kids)
  ; (-> string? list? tree?)  ; (λ (the-subtrees) (andmap tree? the-subtrees))  "andmap" checks if all list elements fit the predicate
  (list tree value kids))
  
(define/contract (tree-root-value a-tree)
  (-> tree? string?)
  (second a-tree))

(define/contract (tree-children   a-tree)
  (-> tree? hedge?)
  (third  a-tree))
   
(define/contract (leaf value)
  (-> string? tree?)
  (tree value (list)))

(define/contract (leaf?  maybe-a-leaf)
  (-> tree? boolean?)
  (empty? (tree-children maybe-a-leaf)))

(define/contract (tree-n-children a-tree)
  (-> tree? integer?)
  (length (tree-children a-tree)))
  
(define/contract (tree-child a-tree i)
  (-> tree? integer? tree?)  ;; ToDo: figure out how to put 0 <= i < (length a-tree) into the contract
  (list-ref (tree-children a-tree) i))

(define/contract (tree=? t1 t2)
  (-> tree? tree? boolean?)
  (and (string=? (tree-root-value t1) (tree-root-value t2))
       (= (tree-n-children t1) (tree-n-children t2)) ; else andmap is so so sad
       (andmap tree=? (tree-children t1) (tree-children t2))))


