% Document Class LaTeX para la Tesis de UTEC.
% Refactorizado para un diseño moderno, diagonal y con tipografía Serif clásica.

\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{tesisutec}

%% ============================================================================
%  Configuración de la clase
%% ============================================================================

\def\baseclass{book}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{\baseclass}}
\def\@checkoptions#1#2{
  \edef\@curroptions{\@ptionlist{\@currname.\@currext}}
  \@tempswafalse
  \@tfor\@this:=#2\do{
    \@expandtwoargs\in@{,\@this,}{,\@curroptions,}
    \ifin@ \@tempswatrue \@break@tfor \fi}
  \let\@this\@empty
  \if@tempswa \else \PassOptionsToClass{#1}{\baseclass}\fi
}
\@checkoptions{11pt}{{10pt}{11pt}{12pt}}
\PassOptionsToClass{a4paper}{\baseclass}
\ProcessOptions\relax
\LoadClass{\baseclass}

% Configurar tamaño del titulo de los capítulos
\def\@makechapterhead#1{%
  {\parindent \z@ \centering
    \normalfont
    \ifnum \c@secnumdepth >\m@ne
    \if@mainmatter
    \Large\bfseries \@chapapp \space \thechapter
    \par\nobreak
    \fi
    \fi
    \interlinepenalty\@M
    \Large \bfseries #1\par\nobreak
    \vskip 40\p@
    \thispagestyle{empty}
  }}

\renewcommand{\@chapapp}{\MakeUppercase{CAPÍTULO}} 

%% ============================================================================
%  Paquetes y comandos para el documento
%% ============================================================================

\usepackage[utf8]{inputenc}
\PassOptionsToPackage{english}{babel}
\PassOptionsToPackage{spanish,es-tabla,es-nodecimaldot}{babel}
\RequirePackage{babel}
\RequirePackage{xcolor}
\RequirePackage{tikz} % Necesario para la geometría de la portada
\usetikzlibrary{calc}

% Colores corporativos (El celeste de la referencia)
\definecolor{utecblue}{RGB}{0, 163, 224} 
\definecolor{darkgray}{RGB}{90, 90, 90}

\RequirePackage{titlesec}
\RequirePackage[subfigure]{tocloft}

\newcommand{\orcid}[1]{\href{https://orcid.org/#1}{\includegraphics[width=10pt]{./images/ORCID128px.png}}}

\newcommand\customchapter[1]{
\chapter*{\center \Large \vspace{-4.5cm} #1 }
\addcontentsline{toc}{section}{\bfseries #1 }
\markboth{#1}{#1} 
}

\newcommand{\introsection}[1]{
{\section*{\hspace{2em}#1}}
\addcontentsline{toc}{section}{\bfseries #1 }
}

\addtocounter{secnumdepth}{1}
\setcounter{tocdepth}{6}
\newcounter{dummy}

\newcommand\addtotoc[1]{
\refstepcounter{dummy}
\addcontentsline{toc}{chapter}{#1}}

\addto\captionsspanish{  \renewcommand{\contentsname}%
    {\hfill\bfseries \Large{TABLA DE CONTENIDO} }%
}

\cftsetindents{section}{0em}{2em}
\cftsetindents{subsection}{0em}{2.5em}

\renewcommand\cftaftertoctitle{\hfill\mbox{}}
\renewcommand{\cftchapfont}{\bfseries CAPÍTULO }
\addtocontents{toc}{\hfill{Pág.}\par}

\renewcommand{\thechapter}{\Roman{chapter}}
\renewcommand{\thesection}{\arabic{chapter}.\arabic{section}}

\newcommand\bhrule{\typeout{------------------------------------------------------------------------------}}

% ====================================================================
% Configuraciones Listas
% ====================================================================

\renewcommand\listoffigures{
\begin{spacing}{1}{
    \setlength{\parskip}{1pt}
    \if@twocolumn
    \@restonecoltrue\onecolumn
    \else
    \@restonecolfalse
    \fi
    \chapter*{\vspace{-2.0cm}\begin{center}\Large{\textbf{ÍNDICE DE FIGURAS}}\end{center}
      \@mkboth{\MakeUppercase\listfigurename}{\MakeUppercase\listfigurename}}
    \@starttoc{lof}
    \if@restonecol\twocolumn\fi
    \cleardoublepage
}\end{spacing}
}

\addto\captionsspanish{\renewcommand{\listfigurename}{\hfill\bfseries\Large{ÍNDICE DE FIGURAS}\hfill }}
\renewcommand{\thefigure}{\arabic{chapter}.\arabic{figure}}
\renewcommand{\cftfigfont}{Figura }

\renewcommand\listoftables{
  \begin{spacing}{1}{
    \setlength{\parskip}{1pt}
    \if@twocolumn
    \@restonecoltrue\onecolumn
    \else
    \@restonecolfalse
    \fi
    \chapter*{\vspace{-2.0cm}\begin{center}\Large{\textbf{ÍNDICE DE TABLAS}}\end{center}
      \@mkboth{\MakeUppercase\listtablename}{\MakeUppercase\listtablename}}
    \@starttoc{lot}
    \if@restonecol\twocolumn\fi
    \cleardoublepage
}\end{spacing}
}

\addto\captionsspanish{\renewcommand{\listtablename}{\hfill\bfseries \Large{ÍNDICE DE TABLAS}\hfill }}
\renewcommand{\thetable}{\arabic{chapter}.\arabic{table}}
\renewcommand{\cfttabfont}{Tabla }

%% ============================================================================
%  Configuración del documento
%% ============================================================================

\newcommand\btypeout[1]{\bhrule\typeout{\space #1}\bhrule}
\usepackage{setspace}
\onehalfspacing
\setlength{\parindent}{1.25cm}
\setlength{\parskip}{2.0ex plus0.5ex minus0.2ex}
\usepackage{vmargin}
\setmarginsrb{3.0cm}{3.0cm}{3.0cm}{3.0cm}{20pt}{0.25in}{9pt}{0.3in}
\raggedbottom
\setlength{\topskip}{1\topskip \@plus 5\p@}
\doublehyphendemerits=10000       
\brokenpenalty=10000              
\widowpenalty=9999                
\clubpenalty=9999                 
\interfootnotelinepenalty=9999    
\usepackage{fancyhdr}
\chead{}\rfoot{\thepage}\lfoot{}\cfoot{}
\renewcommand{\headrulewidth}{0pt} 
\fancyhead{}
\pagestyle{fancy}
\renewcommand{\chaptermark}[1]{\btypeout{\thechapter\space #1}\markboth{\@chapapp\ \thechapter\ #1}{\@chapapp\ \thechapter\ #1}}
\renewcommand{\sectionmark}[1]{}
\renewcommand{\subsectionmark}[1]{}
\def\cleardoublepage{\clearpage\if@twoside \ifodd\c@page\else
\hbox{}
\thispagestyle{empty}
\newpage
\if@twocolumn\hbox{}\newpage\fi\fi\fi}
\usepackage{amsmath,amsfonts,amssymb,amscd,amsthm,xspace}
\theoremstyle{plain}
\newtheorem{example}{Example}[chapter]
\newtheorem{theorem}{Theorem}[chapter]
\theoremstyle{definition}
\newtheorem{definition}[theorem]{Definition}
\usepackage[centerlast,small,sc]{caption}
\setlength{\captionmargin}{20pt}
\newcommand{\fref}[1]{Figure~\ref{#1}}
\newcommand{\tref}[1]{Table~\ref{#1}}
\newcommand{\eref}[1]{Equation~\ref{#1}}
\newcommand{\cref}[1]{Chapter~\ref{#1}}
\newcommand{\sref}[1]{Section~\ref{#1}}
\newcommand{\aref}[1]{Appendix~\ref{#1}}
\renewcommand{\topfraction}{0.85}
\renewcommand{\bottomfraction}{.85}
\renewcommand{\textfraction}{0.1}
\renewcommand{\dbltopfraction}{.85}
\renewcommand{\floatpagefraction}{0.75}
\renewcommand{\dblfloatpagefraction}{.75}
\setcounter{topnumber}{9}
\setcounter{bottomnumber}{9}
\setcounter{totalnumber}{20}
\setcounter{dbltopnumber}{9}
\usepackage{graphicx}
\usepackage{epstopdf}
\usepackage[scriptsize]{subfigure}
\usepackage{booktabs}
\usepackage{rotating}
\usepackage{listings}

\usepackage[pdfpagemode={UseOutlines},bookmarks=true,bookmarksopen=true,
   bookmarksopenlevel=0,bookmarksnumbered=true,hypertexnames=false,
   colorlinks,linkcolor={black},citecolor={blue},urlcolor={red},
   pdfstartview={FitV},unicode,breaklinks=true]{hyperref}
\pdfstringdefDisableCommands{
   \let\\\space
   \def\includegraphics#1{}
   \def\orcid#1{}
}

\newcommand*{\supervisor}[1]{\def\supname{#1}}
\newcommand*{\examiner}[1]{\def\examname{#1}}
\newcommand*{\examinertwo}[1]{\def\examnametwo{#1}}
\newcommand*{\president}[1]{\def\presname{#1}}
\newcommand*{\major}[1]{\def\majorname{#1}}
\newcommand*{\degree}[1]{\def\degreename{#1}}
\renewcommand*{\author}[1]{\def\authornames{#1}}
\newcommand*{\authorid}[1]{\def\studentid{#1}}
\newcommand*{\department}[1]{\def\deptname{#1}}
\newcommand*{\subject}[1]{\def\subjectname{#1}}
\newcommand*{\keywords}[1]{\def\keywordnames{#1}}
\renewcommand*{\date}[1]{\def\dateMY{#1}}
\supervisor  {}
\examiner    {}
\major      {}
\degree      {}
\author      {}
\subject     {}
\keywords    {}

\titleformat{\section}
  {\normalfont \bfseries}{\thesection}{1em}{}
\titleformat{\subsection}
  {\normalfont \bfseries}{\thesubsection}{1em}{}
\setlength{\cftchapnumwidth}{3em}
  
%% ====================================================================
%% Portada de la tesis (Diseño Geométrico Diagonal, Fuente Serif Clásica)
%% ============================================================================
\renewcommand\maketitle{
  \btypeout{Title Page}
  \hypersetup{pdftitle={\@title}, pdfsubject=\subjectname, pdfauthor=\authornames, pdfkeywords=\keywordnames}
  \thispagestyle{empty}
  \begin{titlepage}
    \let\footnotesize\small
    \let\footnoterule\relax
    \let \footnote \thanks
    \setcounter{footnote}{0}

    % Renderizado del fondo geométrico usando TikZ
    \begin{tikzpicture}[remember picture, overlay]
      % Polígono Negro Principal (Alineado al vértice superior y extendido en la base)
      \fill[black] (current page.north west) -- (current page.south west) -- ($(current page.south west) + (11.5cm, 0)$) -- cycle;
      % Franja Diagonal UTEC Blue
      \fill[utecblue] (current page.north west) -- ($(current page.south west) + (11.5cm, 0)$) -- ($(current page.south west) + (13.5cm, 0)$) -- cycle;
      % Logo en blanco anclado abajo a la izquierda (Asegúrate de que el archivo exista en "images/")
      \node[anchor=south west, inner sep=0pt, xshift=3cm, yshift=3cm] at (current page.south west) {\includegraphics[width=5cm]{images/logo-utec-white.png}};
    \end{tikzpicture}
    
    % Contenedor de la información, alineado a la derecha
    \begin{flushright}
      \vspace*{0.5cm}
      {\Large \bfseries UNIVERSIDAD DE INGENIERÍA Y TECNOLOGÍA}\\[0.2cm]
      {\normalsize \color{darkgray} CARRERA DE \MakeUppercase{\deptname}}\\[3cm]

      % Bloque del Título (Limita el ancho para que no choque con la diagonal)
      \begin{minipage}{0.85\textwidth}
        \begin{flushright}
          {\Huge \bfseries \@title \par}
          \vspace{1.5cm}
          {\Large \bfseries TRABAJO DE INVESTIGACIÓN\par}
          \vspace{0.3cm}
          {\large \color{darkgray} Para optar el grado de \degreename\ en \majorname \par}
        \end{flushright}
      \end{minipage}\\[2.5cm]

      % Bloque de Autor y Asesor (Aún más a la derecha en la base)
      \begin{minipage}{0.6\textwidth}
        \begin{flushright}
          {\normalsize \bfseries \color{utecblue} AUTOR(ES)}\\[0.2cm]
          {\large \authornames}\\[0.8cm]

          {\normalsize \bfseries \color{utecblue} ASESOR(ES)}\\[0.2cm]
          {\large \supname}\\[1.5cm]

          {\large \bfseries Lima - Perú}\\[0.1cm]
          {\large \bfseries \dateMY}
        \end{flushright}
      \end{minipage}
      
    \end{flushright}
  \end{titlepage}
  
  \setcounter{footnote}{0}%
  \global\let\thanks\relax
  \global\let\maketitle\relax
  \global\let\@thanks\@empty
  \global\let\@author\@empty
  \global\let\title\relax
  \global\let\author\relax
  \global\let\and\relax
  \cleardoublepage
}

% ====================================================================
% Firmas, Dedicatoria y Agradecimientos
%% ============================================================================

\newcommand\Aprobado[1]{
  \setstretch{1.5}  
  \thispagestyle{empty}
  \noindent La tesis\\
  \textbf{\@title} \\
  ha sido aprobada.
  {\normalsize #1}

  \vspace{2.5cm}
  \hfill\rule[0.5em]{16.5em}{0.5pt} \\  
  ${~}$\hfill \presname \\

  \vspace{1.5cm}
  \hfill\rule[0.5em]{16.5em}{0.5pt} \\  
  ${~}$\hfill \supname \\
  
  \vspace{1.5cm}
  \hfill\rule[0.5em]{16.5em}{0.5pt} \\  
  ${~}$\hfill \examname \\

  \vspace{1.5cm}
  \hfill\rule[0.5em]{16.5em}{0.5pt} \\  
  ${~}$\hfill \examnametwo \\
  \clearpage
}

\newcommand\dedicatory[1]{
  \setstretch{1.0}  
  \thispagestyle{empty}
  \null\vfil
  \vskip 60\p@
  \vspace{4.5cm}
  ${~}$\hfill \textit{Dedicatoria}: \\
  \begin{flushright}{ \sl #1}\end{flushright}
  \vfil\null
  \cleardoublepage
  \setstretch{1.5} 
}

\newcommand\acknowledgements[1]{
  \setstretch{1.0} 
  \thispagestyle{empty}
  \null\vfil
  \vskip 60\p@
  \vspace{4.5cm}
  ${~}$\hfill \textit{Agradecimientos}: \\
  \begin{flushright}{ \sl #1}\end{flushright}
  \vfil\null
  \cleardoublepage
  \setstretch{1.5} 
}

\newcommand\resumen[1]{
\btypeout{Resumen}
\addtotoc{Resumen}
\thispagestyle{plain}
\null\vfil
\begin{center}{\huge\bfseries Resumen\par}\end{center}
{\normalsize #1}
\vfil\vfil\null
}

\newcommand\abstract[1]{
\btypeout{Abstract}
\addtotoc{Abstract}
\thispagestyle{plain}
\null\vfil
\begin{center}{\huge\bfseries Abstract\par}\end{center}
{\normalsize #1}
\vfil\vfil\null
}

\newcommand\recommendations[1]{
  \btypeout{RECOMENDACIONES}
  \addtotoc{RECOMENDACIONES}
  \clearpage
  \thispagestyle{plain}
  \begin{center}{\Large \textbf{RECOMENDACIONES} \par}\end{center}
  \vspace{0.5cm}
  {\normalsize #1}
  \vfil\vfil\null
  \clearpage
}

\endinput
%% End of file `tesisutec.cls'.