#include "geesespotter_lib.h"
#include <iostream>

bool inBounds(int x,int y, std::size_t x_dim, std::size_t y_dim);



char *create_board(std::size_t x_dim, std::size_t y_dim){
    std::size_t capacity = x_dim*y_dim; 
    char* array = new char[capacity];

    for(int i{}; i<capacity; i++){
        array[i]=0;
    }

    return array;
}       

void clean_board(char * board){
    delete[] board; 
    board = nullptr;
    return; 
}


void print_board(char *board, std::size_t x_dim, std::size_t y_dim) {
    for (std::size_t y{0}; y < y_dim; ++y) {
        for (std::size_t x{0}; x < x_dim; ++x) {

            char tile = board[y*x_dim + x];

            if ((tile & marked_mask())==marked_mask()) {
                std::cout << "M";
            } else if ((tile & hidden_mask())==hidden_mask()) {
                std::cout << "*";
            } else{
                std::cout<< (int) (tile&value_mask());
            }

        }
        std::cout<<std::endl;
    }
}


void hide_board(char *board, std::size_t x_dim, std::size_t y_dim){
    for(int i{}; i< (x_dim*y_dim); i++){
        board[i]|= hidden_mask();
    }
    return;
}

int mark(char *board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc,
std::size_t y_loc){
    char tile = board[y_loc*x_dim + x_loc];
    
    if((tile&hidden_mask())!=hidden_mask()){
        return 2;
        } else{
            board[y_loc*x_dim + x_loc]^=marked_mask();
            return 0;
            }
}

    
void compute_neighbours(char *board, std::size_t x_dim, std::size_t y_dim){

    for (int y{0}; y < y_dim; ++y){
        for (int x{0}; x < x_dim; ++x){
            char tile = board[y*x_dim + x];
            if(tile != 9){
                int field_value{0};
                if( inBounds (x-1, y+1, x_dim, y_dim) == true){
                    if(board[(y+1)*x_dim+(x-1)]==9){
                        field_value = 1+ field_value;
                    }
                }
                
                if (inBounds (x-1, y, x_dim, y_dim) == true){
                    if(board[(y)*x_dim+(x-1)]==9){
                        field_value = 1+ field_value;
                    }
                }
                
                if (inBounds (x-1, y-1, x_dim, y_dim) == true){
                    if(board[(y-1)*x_dim+(x-1)]==9){
                        field_value = 1+ field_value;
                    }
                }

                if (inBounds (x, y-1, x_dim, y_dim)== true){
                    if(board[(y-1)*x_dim+(x)]==9){
                        field_value = 1+ field_value;
                    }
                }

                if (inBounds (x + 1, y +1, x_dim, y_dim) == true){
                    if(board[(y+1)*x_dim+(x+1)]==9){
                        field_value = 1+ field_value;
                    }
                }

                if (inBounds (x+ 1, y, x_dim, y_dim)== true){
                    if(board[(y)*x_dim+(x+1)]==9){
                        field_value = 1+ field_value;
                    }
                }

                if (inBounds (x, y+1, x_dim, y_dim)==true){
                    if(board[(y+1)*x_dim+(x)]==9){
                        field_value = 1+ field_value;
                    }
                }
                
                if( inBounds (x +1, y -1, x_dim, y_dim) == true){
                    if(board[(y-1)*x_dim+(x+1)]==9){
                        field_value = 1+ field_value;
                    }
                }
                board[y*x_dim+ x] = field_value; 
            }    

             
        }
    }

    return;
}

bool inBounds(int x,int y, std::size_t x_dim, std::size_t y_dim){
    if(y<0||y >= (int)y_dim){
        return false;
    }
        //not within bounds for y 
                
    if(x<0||x>= (int)x_dim){
        return false;
    } 

    return true;
}

bool is_game_won(char *board, std::size_t x_dim, std::size_t y_dim){
    for (std::size_t y{0}; y < y_dim; ++y){
        for (std::size_t x{0}; x < x_dim; ++x){
            if(((board[y*x_dim+x] & hidden_mask())==hidden_mask()) && ((board[y*x_dim+x] & value_mask()) != 9)){
                return false; 
            }
        }
    }

    return true;
}

int reveal(char *board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc,
std::size_t y_loc){
    //char tile = board [ y_loc*x_dim+x_loc];

    if ((board[y_loc*x_dim+x_loc] & marked_mask()) == marked_mask()){
        return 1;
    }

    if ((board[y_loc*x_dim+x_loc] & hidden_mask())!= hidden_mask()){
        return 2;
    }

    if ((board[y_loc*x_dim+x_loc]&value_mask())==9){
        board[(y_loc)*x_dim+(x_loc)]^=hidden_mask();
        return 9; 
    }

    board[(y_loc)*x_dim+(x_loc)]^=hidden_mask();

    if (((board[y_loc*x_dim+x_loc] & value_mask())== 0)){
        if( (inBounds (x_loc-1, y_loc+1, x_dim, y_dim)) == true){
            if((board[(y_loc+1)*x_dim+(x_loc-1)]&hidden_mask())==0){
                return 0;
            } else {
                board[(y_loc+1)*x_dim+(x_loc-1)]^=hidden_mask();
            }
        }
                
        if ((inBounds (x_loc-1, y_loc, x_dim, y_dim)) == true){
            if((board[(y_loc)*x_dim+(x_loc-1)]&hidden_mask())==0){
                return 0;
            } else {
                board[(y_loc)*x_dim+(x_loc-1)]^=hidden_mask();
            }
        }
                
        if ((inBounds (x_loc-1, y_loc-1, x_dim, y_dim)) == true){
            if((board[(y_loc-1)*x_dim+(x_loc-1)]&hidden_mask()) == 0){
                return 0;
            } else {
                board[(y_loc-1)*x_dim+(x_loc-1)]^=hidden_mask();
            }
        }

        if ((inBounds (x_loc, y_loc-1, x_dim, y_dim))== true){
            if((board[(y_loc-1)*x_dim+(x_loc)]&hidden_mask())==0){
                return 0;
            } else{
                board[(y_loc-1)*x_dim+(x_loc)]^=hidden_mask();
            }
        }

        if ((inBounds (x_loc + 1, y_loc +1, x_dim, y_dim)) == true){
            if ((board[(y_loc+1)*x_dim+(x_loc+1)]&hidden_mask())==0){
                return 0;
            } else {
                board[(y_loc+1)*x_dim+(x_loc+1)]^=hidden_mask();
            }
        }

        if ((inBounds (x_loc+ 1, y_loc, x_dim, y_dim))== true){
            if((board[(y_loc)*x_dim+(x_loc+1)]&hidden_mask())==0){
                return 0; 
            } else{
                board[(y_loc)*x_dim+(x_loc+1)]^=hidden_mask();
            }
        }

        if ((inBounds (x_loc, y_loc+1, x_dim, y_dim))==true){
            if((board[(y_loc+1)*x_dim+(x_loc)]&hidden_mask())==0){
                return 0;
            } else {
                board[(y_loc+1)*x_dim+(x_loc)]^=hidden_mask();
            }
        }
                
        if( (inBounds (x_loc +1, y_loc -1, x_dim, y_dim)) == true){
            if((board[(y_loc-1)*x_dim+(x_loc-+1)]&hidden_mask())==0){
                return 0;
            } else{
                board[(y_loc-1)*x_dim+(x_loc-+1)]^=hidden_mask();
            }

        }

        else{
            return 0;
        }
    }
    return 0;

}        
    

