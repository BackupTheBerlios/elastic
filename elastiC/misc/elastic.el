;;
;; elastiC for Emacs
;;
;; Copyright (C) 2001 Marco Pantaleoni. All rights reserved.
;;

(require 'cc-mode)
(require 'font-lock)
(provide 'elastic)

(defvar elastic-mode-syntax-table nil "\
Syntax table used in elastic-mode buffers.")

(autoload 'elastic-mode "cc-mode" "\
Major mode for editing elastiC code.
To submit a problem report, enter `\\[c-submit-bug-report]' from an
elastic-mode buffer.  This automatically sets up a mail buffer with
version information already added.  You just need to add a description
of the problem, including a reproducible test case, and send the
message.
 
To see what version of CC Mode you are running, enter `\\[c-version]'.
 
The hook variable `elastic-mode-hook' is run with no args, if that value
is bound and has a non-nil value.  Also the common hook
`c-mode-common-hook' is run first.
 
Key bindings:
\\{elastic-mode-map}" t nil)

;; START OF cc-langs

(defconst c-qualified-symbol-key "[_a-zA-Z]\\(\\w\\|\\s_\\|\\.\\)*")

(defconst c-elastiC-class-key
  (concat
   "\\(private\\|public\\)\\s +class\\s +"
   c-symbol-key				;name of the class
   "\\s *extends\\s *" c-qualified-symbol-key	;followed by the superclass
   ))

;; regexp describing access protection clauses.  language specific
(defconst c-elastiC-access-key nil)

;; keywords introducing conditional blocks
(defconst c-elastiC-conditional-key nil)

(let ((all-kws "for\\|if\\|do\\|else\\|while\\|switch")
      (exc-kws "\\|try\\|catch")
      (thr-kws "\\|finally")
      (front   "\\b\\(")
      (back    "\\)\\b[^_]"))
  (setq c-elastiC-conditional-key (concat front all-kws exc-kws thr-kws back)))

;; Regexp that starts lambda constructs.
(defvar c-lambda-key nil)
(make-variable-buffer-local 'c-lambda-key)
(defconst c-elastiC-lambda-key "\\<function\\>")

;; Regexp that are followed by a statement block in expressions.
(defvar c-inexpr-block-key nil)
(make-variable-buffer-local 'c-inexpr-block-key)
(defconst c-elastiC-inexpr-block-key "\\<catch\\>")

(defvar c-elastic-menu nil)


;; Support for elastiC

(defvar elastic-mode-abbrev-table nil
  "Abbreviation table used in elastic-mode buffers.")
(define-abbrev-table 'elastic-mode-abbrev-table ())

(defvar elastic-mode-map ()
  "Keymap used in elastic-mode buffers.")
(if elastic-mode-map
    nil
  (setq elastic-mode-map (c-make-inherited-keymap))
  ;; additional bindings
  (define-key elastic-mode-map "\C-c\C-e" 'c-macro-expand))

;;;###autoload
(defvar elastic-mode-syntax-table nil
  "Syntax table used in elastic-mode buffers.")
(if elastic-mode-syntax-table
    ()
  (setq elastic-mode-syntax-table (make-syntax-table))
  (c-populate-syntax-table elastic-mode-syntax-table)
  (modify-syntax-entry ?@ "." elastic-mode-syntax-table))

(easy-menu-define c-elastic-menu elastic-mode-map "elastiC Mode Commands"
		  (c-mode-menu "elastiC"))

;; END OF cc-langs

;; START OF cc-mode


;;;###autoload
(defun elastic-mode ()
  "Major mode for editing elastiC code.
To submit a problem report, enter `\\[c-submit-bug-report]' from an
elastic-mode buffer.  This automatically sets up a mail buffer with
version information already added.  You just need to add a description
of the problem, including a reproducible test case, and send the
message.

To see what version of CC Mode you are running, enter `\\[c-version]'.

The hook variable `elastic-mode-hook' is run with no args, if that value
is bound and has a non-nil value.  Also the common hook
`c-mode-common-hook' is run first.

Key bindings:
\\{elastic-mode-map}"
  (interactive)
  (c-initialize-cc-mode)
  (kill-all-local-variables)
  (set-syntax-table elastic-mode-syntax-table)
  (setq major-mode 'elastic-mode
 	mode-name "elastiC"
 	local-abbrev-table elastic-mode-abbrev-table)
  (use-local-map elastic-mode-map)
  (c-common-init)
  (setq comment-start "// "
 	comment-end   ""
 	c-conditional-key c-elastiC-conditional-key
  	c-class-key c-elastiC-class-key
	c-method-key nil
 	c-baseclass-key nil
	c-recognize-knr-p nil
 	c-access-key c-elastiC-access-key
	c-lambda-key c-elastiC-lambda-key
	c-inexpr-block-key c-elastiC-inexpr-block-key
	;c-special-brace-lists c-elastiC-special-brace-lists
	;imenu-generic-expression cc-imenu-java-generic-expression ;FIXME
	;imenu-case-fold-search nil ;FIXME
	)
  (run-hooks 'c-mode-common-hook)
  (run-hooks 'elastic-mode-hook)
  (c-update-modeline))

;; END OF cc-mode

;; font locking

(put 'elastic-mode 'font-lock-defaults
     '((elastic-font-lock-keywords
        elastic-font-lock-keywords-1 elastic-font-lock-keywords-2
        elastic-font-lock-keywords-3)
       nil nil ((?_ . "w")) beginning-of-defun
       (font-lock-mark-block-function . mark-defun)))


;;; elastiC.

(defconst elastic-font-lock-keywords-1 nil
 "For consideration as a value of `elastic-font-lock-keywords'.
This does fairly subdued highlighting.")

(defconst elastic-font-lock-keywords-2 nil
 "For consideration as a value of `elastic-font-lock-keywords'.
This adds highlighting of types and identifier names.")

(defconst elastic-font-lock-keywords-3 nil
 "For consideration as a value of `elastic-font-lock-keywords'.
This adds highlighting of elastiC documentation tags, such as @see.")

(defvar elastic-font-lock-type-regexp
  (concat "\\<\\(boolean\\|byte\\|char\\|double\\|float\\|int"
         "\\|long\\|short\\|void\\)\\>")
  "Regexp which should match a primitive type.")

(let ((capital-letter "A-Z")
      (letter "a-zA-Z_")
      (digit  "0-9"))
(defvar elastic-font-lock-identifier-regexp
  (concat "\\<\\([" letter "][" letter digit "]*\\)\\>")
  "Regexp which should match all elastiC identifiers.")

(defvar elastic-font-lock-class-name-regexp
  (concat "\\<\\([" letter "][" letter digit "]*\\)\\>")
  "Regexp which should match a class name.")
)


(let ((elastic-modifier-regexp
       (concat "\\<\\(local\\|static\\|private\\|public\\)\\>")))

  ;; Basic font-lock support:
  (setq elastic-font-lock-keywords-1
	(list
	 ;; Keywords:
	 (list        
	  (concat
	   "\\<\\("
	   "break\\|"
	   "case\\|catch\\|class\\|continue\\|"
	   "do\\|else\\|extends\\|"
	   "finally\\|for\\|from\\|function\\|"
	   "goto\\|"
	   "if\\|import\\|in\\|"
	   "method\\|"
	   "package\\|"
	   "return\\|"
	   "switch\\|"
	   "throw\\|try\\|"
	   "while\\)\\>")
	  1 'font-lock-keyword-face)

	 ;; Modifiers:
	 (list elastic-modifier-regexp 1 font-lock-type-face)

	 ;; Special constants:
	 '("\\<\\(self\\|super\\)\\>" (1 font-lock-reference-face))
	 '("\\<\\(@false\\|@nil\\|@true\\)\\>" (1 font-lock-keyword-face))

	 ;; Class names:
	 (list (concat "\\<\\(class\\)\\>\\s *" 
		       elastic-font-lock-identifier-regexp)
	       2 'font-lock-function-name-face)
        
	 ;; Package declarations:
	 (list (concat "\\<\\(package\\|import\\)\\>\\s *"
		       elastic-font-lock-identifier-regexp)
	       '(2 font-lock-reference-face)
	       (list (concat
		      "\\=\\.\\(" elastic-font-lock-identifier-regexp "\\)")
		     nil nil '(1 (if (equal (char-after (match-end 0)) ?.)
				     'font-lock-reference-face
				   'font-lock-type-face))))
	 
	 ;; Constructors:
	 (list (concat
		"^\\s *\\(" elastic-modifier-regexp "\\s +\\)*"
		elastic-font-lock-class-name-regexp "\\s *\(")
	       (list 3
		     '(condition-case nil
			  (save-excursion
			    (goto-char (scan-sexps (- (match-end 0) 1) 1))
			    (parse-partial-sexp (point) (point-max) nil t)
			    (and (looking-at "\\($\\|\\<throws\\>\\|{\\)")
				 'font-lock-function-name-face))
			(error 'font-lock-function-name-face))))

	 ;; Methods:
	 (list (concat "\\(" elastic-font-lock-type-regexp "\\|"
		       elastic-font-lock-class-name-regexp "\\)"
		       "\\s *\\(\\[\\s *\\]\\s *\\)*"
		       elastic-font-lock-identifier-regexp "\\s *\(")
	       5
	       'font-lock-function-name-face)

	 ;; Labels:
	 (list ":"
	       (list
		(concat "^\\s *" elastic-font-lock-identifier-regexp "\\s *:")
		'(beginning-of-line) '(end-of-line)
		'(1 font-lock-reference-face)))

	 ;; `break' and continue' destination labels:
	 (list (concat "\\<\\(break\\|continue\\)\\>\\s *"
		       elastic-font-lock-identifier-regexp)
	       2 'font-lock-reference-face)

	 ;; Case statements:
	 ;; In elastiC, any constant expression is allowed.
	 '("\\<case\\>\\s *\\(.*\\):" 1 font-lock-reference-face)))

  ;; Types and declared variable names:
  (setq elastic-font-lock-keywords-2
	(append 

	 elastic-font-lock-keywords-1
	 (list
	  ;; Keywords followed by a type:
	  (list (concat "\\<\\(extends\\|instanceof\\|new\\)\\>\\s *"
			elastic-font-lock-identifier-regexp)
		'(2 (if (equal (char-after (match-end 0)) ?.)
			'font-lock-reference-face 'font-lock-type-face))
		(list (concat "\\=\\." elastic-font-lock-identifier-regexp)
		      '(goto-char (match-end 0)) nil
		      '(1 (if (equal (char-after (match-end 0)) ?.)
			      'font-lock-reference-face 'font-lock-type-face))))

	  ;; Keywords followed by a type list:
	  (list (concat "\\<\\(implements\\|throws\\)\\>\\ s*"
			elastic-font-lock-identifier-regexp)
		'(2 (if (equal (char-after (match-end 0)) ?.)
			font-lock-reference-face font-lock-type-face))
		(list (concat "\\=\\(\\.\\|\\s *\\(,\\)\\s *\\)"
			      elastic-font-lock-identifier-regexp)
		      '(goto-char (match-end 0)) nil
		      '(3 (if (equal (char-after (match-end 0)) ?.)
			      font-lock-reference-face font-lock-type-face))))

	  ;; primitive types, can't be confused with anything else.
	  (list elastic-font-lock-type-regexp
		'(1 font-lock-type-face)
		'(font-lock-match-elastic-declarations
		  (goto-char (match-end 0))
		  (goto-char (match-end 0))
		  (0 font-lock-variable-name-face)))

	  ;; Declarations, class types and capitalized variables:
	  ;;
	  ;; Declarations are easy to recognize.  Capitalized words
	  ;; followed by a closing parenthesis are treated as casts if they
	  ;; also are followed by an expression.  Expressions beginning with
	  ;; a unary numerical operator, e.g. +, can't be cast to an object
	  ;; type.
	  ;;
	  ;; The path of a fully qualified type, e.g. elastic.lang.Foo, is
	  ;; fontified in the reference face.
	  ;;
	  ;; An access to a static field, e.g. System.out.println, is
	  ;; not fontified since it can't be distinguished from the
	  ;; usage of a capitalized variable, e.g. Foo.out.println.

	  (list (concat elastic-font-lock-class-name-regexp
			"\\s *\\(\\[\\s *\\]\\s *\\)*"
			"\\(\\<\\|$\\|)\\s *\\([\(\"]\\|\\<\\)\\)")
		'(1 (save-match-data
		      (save-excursion
			(goto-char
			 (match-beginning 3))
			(if (not (looking-at "\\<instanceof\\>"))
			    'font-lock-type-face))))
		(list (concat "\\=" elastic-font-lock-identifier-regexp "\\.")
		      '(progn
			 (goto-char (match-beginning 0))
			 (while (or (= (preceding-char) ?.)
				    (= (char-syntax (preceding-char)) ?w))
			   (backward-char)))
		      '(goto-char (match-end 0))
		      '(1 font-lock-reference-face)
		      '(0 nil))		; Workaround for bug in XEmacs.
		'(font-lock-match-elastic-declarations
		  (goto-char (match-end 1))
		  (goto-char (match-end 0))
		  (1 font-lock-variable-name-face))))))

  ;; Modifier keywords and elastiC doc tags
  (setq elastic-font-lock-keywords-3
	(append

	 '(
	   ;; Feature scoping:
	   ;; These must come first or the Modifiers from keywords-1 will
	   ;; catch them.  We don't want to use override fontification here
	   ;; because then these terms will be fontified within comments.
	   ("\\<\\(private\\|static\\)\\>"   0 font-lock-string-face)
	   ;("\\<local\\>" 0 font-lock-preprocessor-face)
	   ("\\<public\\>"    0 font-lock-reference-face))
	 elastic-font-lock-keywords-2

	 (list

	  ;; elasticdoc tags
	  '("@\\(author\\|exception\\|throws\\|deprecated\\|param\\|return\\|see\\|since\\|version\\)\\s "
	    0 font-lock-keyword-face t)

	  ;; Doc tag - Parameter identifiers
	  (list (concat "@param\\s +" elastic-font-lock-identifier-regexp)
		1 'font-lock-variable-name-face t)

	  ;; Doc tag - Exception types
	  (list (concat "@\\(exception\\|throws\\)\\s +"
			elastic-font-lock-identifier-regexp)
		'(2 (if (equal (char-after (match-end 0)) ?.)
			font-lock-reference-face font-lock-type-face) t)
		(list (concat "\\=\\." elastic-font-lock-identifier-regexp)
		      '(goto-char (match-end 0)) nil
		      '(1 (if (equal (char-after (match-end 0)) ?.)
			      'font-lock-reference-face 'font-lock-type-face) t)))

	  ;; Doc tag - Cross-references, usually to methods 
	  '("@see\\s +\\(\\S *[^][ \t\n\r\f(){},.;:]\\)"
	    1 font-lock-function-name-face t)

	  ;; Doc tag - docRoot (1.3)
	  '("\\({ *@docRoot *}\\)"
	    0 font-lock-keyword-face t)
	  ;; Doc tag - beaninfo, unofficial but widely used, even by Sun
	  '("\\(@beaninfo\\)"
	    0 font-lock-keyword-face t)
	  ;; Doc tag - Links
	  '("{ *@link\\s +\\([^}]+\\)}"
	    0 font-lock-keyword-face t)
	  ;; Doc tag - Links
	  '("{ *@link\\s +\\(\\(\\S +\\)\\|\\(\\S +\\s +\\S +\\)\\) *}"
	    1 font-lock-function-name-face t)

	  )))
  )

(defvar elastic-font-lock-keywords elastic-font-lock-keywords-1
  "Additional expressions to highlight in elastiC mode.")

;; Match and move over any declaration/definition item after
;; point.  Does not match items which look like a type declaration
;; (primitive types and class names, i.e. capitalized words.)
;; Should the variable name be followed by a comma, we reposition
;; the cursor to fontify more identifiers.
(defun font-lock-match-elastic-declarations (limit)
  "Match and skip over variable definitions."
  (if (looking-at "\\s *\\(\\[\\s *\\]\\s *\\)*")
      (goto-char (match-end 0)))
  (and
   (looking-at elastic-font-lock-identifier-regexp)
   (save-match-data
     (not (string-match elastic-font-lock-type-regexp
			(buffer-substring (match-beginning 1)
					  (match-end 1)))))
   (save-match-data
     (save-excursion
       (goto-char (match-beginning 1))
       (not (looking-at
	     (concat elastic-font-lock-class-name-regexp
		     "\\s *\\(\\[\\s *\\]\\s *\\)*\\<")))))
   (save-match-data
     (condition-case nil
	 (save-restriction
	   (narrow-to-region (point-min) limit)
	   (goto-char (match-end 0))
	   ;; Note: Both `scan-sexps' and the second goto-char can
	   ;; generate an error which is caught by the
	   ;; `condition-case' expression.
	   (while (not (looking-at "\\s *\\(\\(,\\)\\|;\\|$\\)"))
	     (goto-char (or (scan-sexps (point) 1) (point-max))))
	   (goto-char (match-end 2)))   ; non-nil
       (error t)))))
