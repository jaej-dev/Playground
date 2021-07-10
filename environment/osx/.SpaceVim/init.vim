"=============================================================================
" init.vim --- Entry file for neovim
" Copyright (c) 2016-2020 Wang Shidong & Contributors
" Author: Wang Shidong < wsdjeg@outlook.com >
" URL: https://spacevim.org
" License: GPLv3
"=============================================================================

execute 'source' fnamemodify(expand('<sfile>'), ':h').'/main.vim'

let g:spacevim_colorscheme = 'onedark'
let g:spacevim_colorscheme_bg = 'light'

" disable QuickFix window
let g:ale_set_quickfix = 0
let g:ale_keep_list_window_open = 0

" display 80 column
set colorcolumn=81

" python for neovim
let g:python_host_prog = '/usr/bin/python'
"let g:python2_host_prog = '/usr/local/bin/python'
let g:python3_host_prog = '/usr/local/bin/python3'

" clang-format
map <C-K> :py3f /usr/local/share/clang/clang-format.py<cr>
imap <C-K> <c-o>:py3f /usr/local/share/clang/clang-format.py<cr>

" copy to clipboard
set clipboard=unnamed

" ctags
" set tags=./tags,tags;$HOME
set tags=~/x/playground/cpp/tesla/tags,tags;$HOME

" cscope
if has("cscope")
    set csprg=/usr/local/bin/cscope
    set csto=0
    set cst
    set nocsverb
    if filereadable("~/x/playground/cpp/tesla/cscope.files")                                                  
      cs add ~/x/playground/cpp/tesla/cscope.files
    endif                                                                            

    if $CSCOPE_DB != ""
        cs add $CSCOPE_DB
    endif

    set csverb
    set cscopeverbose
    " cscope/vim key mappings
    " 's' symbol: find all references to the token under cursor
    " 'g' global: find global definition(s) of the token under cursor
    " 'c' calls:  find all calls to the function name under cursor
    " 't' text:   find all instances of the text under cursor
    " 'e' egrep:  egrep search for the word under cursor
    " 'f' file:   open the filename under cursor
    " 'i' includes: find files that include the filename under cursor
    " 'd' called: find functions that function under cursor calls
    nmap <C-\>s :cs find s <C-R>=expand("<cword>")<CR><CR>
    nmap <C-\>g :cs find g <C-R>=expand("<cword>")<CR><CR>
    nmap <C-\>c :cs find c <C-R>=expand("<cword>")<CR><CR>
    nmap <C-\>t :cs find t <C-R>=expand("<cword>")<CR><CR>
    nmap <C-\>e :cs find e <C-R>=expand("<cword>")<CR><CR>
    nmap <C-\>f :cs find f <C-R>=expand("<cword>")<CR><CR>
    nmap <C-\>i :cs find i <C-R>=expand("<cword>")<CR><CR>
    nmap <C-\>d :cs find d <C-R>=expand("<cword>")<CR><CR>

    " CTRL-space <C-@> search and split horizonal window
    nmap <C-@>s :scs find s <C-R>=expand("<cword>")<CR><CR>
    nmap <C-@>g :scs find g <C-R>=expand("<cword>")<CR><CR>
    nmap <C-@>c :scs find c <C-R>=expand("<cword>")<CR><CR>
    nmap <C-@>t :scs find t <C-R>=expand("<cword>")<CR><CR>
    nmap <C-@>e :scs find e <C-R>=expand("<cword>")<CR><CR>
    nmap <C-@>f :scs find f <C-R>=expand("<cword>")<CR><CR>
    nmap <C-@>i :scs find i <C-R>=expand("<cword>")<CR><CR>
    nmap <C-@>d :scs find d <C-R>=expand("<cword>")<CR><CR>

    " CTRL-space CTRL-space vertical split
    nmap <C-@><C-@>s :vert scs find s <C-R>=expand("<cword>")<CR><CR>
    nmap <C-@><C-@>g :vert scs find g <C-R>=expand("<cword>")<CR><CR>
    nmap <C-@><C-@>c :vert scs find c <C-R>=expand("<cword>")<CR><CR>
    nmap <C-@><C-@>t :vert scs find t <C-R>=expand("<cword>")<CR><CR>
    nmap <C-@><C-@>e :vert scs find e <C-R>=expand("<cword>")<CR><CR>
    nmap <C-@><C-@>f :vert scs find f <C-R>=expand("<cword>")<CR><CR>
    nmap <C-@><C-@>i :vert scs find i <C-R>=expand("<cword>")<CR><CR>
    nmap <C-@><C-@>d :vert scs find d <C-R>=expand("<cword>")<CR><CR>

    " key map timeout
    "set notimeout
    " Or
    "set timetouteln=4000
    "set ttimeout
    "set ttimeoutlen=100
endif
