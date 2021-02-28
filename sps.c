#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct{
    int row_start;
    int row_end;
    int col_start;
    int col_end;
} selection_t;

typedef struct{
    char *vars[10];
    selection_t sel;
} temp_vars_t;

typedef struct{
    int cell_len;
    char *cell;
} cell_t;

typedef struct{
    int cell_cnt;
    cell_t *cells;
} row_t;

typedef struct{
    int row_cnt;
    row_t *rows;
} table_t;

// Najdu buňku s nejmenším číslem a nastaví výběr na ni
void find_min(selection_t *selection, table_t *table){
    int tmp_min;
    int min_col;
    int min_row;
    bool is_set = false;
    for(int row = selection->row_start; ((row < selection->row_end) && (row < table->row_cnt)); row++){
        for(int col = selection->col_start; ((col < selection->col_end) && (col < table->rows[row].cell_cnt)); col++){
            if(strcmp(table->rows[row].cells[col].cell,"")){
                char *tmp_str;
                int arg = strtol(table->rows[row].cells[col].cell, &tmp_str, 10);
                if((!strcmp(tmp_str,"")) && (!is_set)){
                    tmp_min = arg;
                    min_row = row;
                    min_col = col;
                    is_set = true;
                }
                else if((!strcmp(tmp_str,"")) && (arg < tmp_min)){
                    tmp_min = arg;
                    min_row = row;
                    min_col = col;
                }
            }
        }
    }
    if(is_set){
        selection->row_start = min_row;
        selection->row_end = min_row;
        selection->col_start = min_col;
        selection->col_end = min_col;
    }
}

//Najde buňku s největším číslem a nastaví výběr na ni
void find_max(selection_t *selection, table_t *table){
    int tmp_max;
    int max_col;
    int max_row;
    bool is_set = false;
    for(int row = selection->row_start; ((row <= selection->row_end) && (row < table->row_cnt)); row++){
        for(int col = selection->col_start; ((col <= selection->col_end) && (col < table->rows[row].cell_cnt)); col++){
            if(strcmp(table->rows[row].cells[col].cell,"")){
                char *tmp_str;
                int arg = strtol(table->rows[row].cells[col].cell, &tmp_str, 10);
                if((!strcmp(tmp_str,"")) && (!is_set)){
                    tmp_max = arg;
                    max_row = row;
                    max_col = col;
                    is_set = true;
                }
                else if((!strcmp(tmp_str,"")) && (arg > tmp_max)){
                    tmp_max = arg;
                    max_row = row;
                    max_col = col;
                }
            }
        }
    }
    if(is_set){
        selection->row_start = max_row;
        selection->row_end = max_row;
        selection->col_start = max_col;
        selection->col_end = max_col;
    }
}

//Najde první buňku, ve které je daný podřetězec a nastaví výber na ni
void find_str(selection_t *selection, table_t *table, char *current_arg){
    char *str_to_find = current_arg+6;
    str_to_find[strlen(str_to_find)-1] = '\0';
    bool found = false;
    for(int row = selection->row_start; ((row <= selection->row_end) && (row < table->row_cnt)); row++){
        for(int col = selection->col_start; ((col <= selection->col_end) && (col < table->rows[row].cell_cnt)); col++){
            if(strstr(table->rows[row].cells[col].cell, str_to_find) != NULL){
                selection->row_start = row;
                selection->row_end = row;
                selection->col_start = col;
                selection->col_end = col;
                found = true;
                break;
            }
        }
        if(found){
            break;
        }
    }
}

//Nastavení výběru pro [R,C], [R,_], [_,C], [_,_], [R1,C1,R2,C2]
void normal_selection(selection_t *selection, table_t *table, char *current_arg, int cells){
    char *arg1 = strtok(current_arg,",")+1;
    char *arg2 = strtok(NULL,",");
    char *arg3 = strtok(NULL,",");
    char *arg4 = strtok(NULL,",");
    if((arg3 == NULL) && (arg4 == NULL)){
        arg2[strlen(arg2)-1] = '\0';
        if((!strcmp(arg1,"_")) || (!strcmp(arg1,"-"))){
            selection->row_start = 0;
            selection->row_end = table->row_cnt-2;
        }
        else{
            char *tmp_str;
            int arg1_int = strtol(arg1, &tmp_str, 10);
            if(!strcmp(tmp_str,"")){
                selection->row_start = arg1_int-1;
                selection->row_end = arg1_int-1;
            }
            else{
                return;
            }
        }
        if((!strcmp(arg2,"_")) ||(!strcmp(arg2,"-"))){
            selection->col_start = 0;
            selection->col_end = cells-1;
        }
        else{
            char *tmp_str;
            int arg2_int = strtol(arg2, &tmp_str, 10);
            if(!strcmp(tmp_str,"")){
                selection->col_start = arg2_int-1;
                selection->col_end = arg2_int-1;
            }
            else{
                return;
            }
        }
    }
    else{
        arg4[strlen(arg4)-1] = '\0';
        char *tmp_str1, *tmp_str2, *tmp_str3, *tmp_str4;
        int arg1_int = strtol(arg1, &tmp_str1, 10);
        int arg2_int = strtol(arg2, &tmp_str2, 10);
        int arg3_int = strtol(arg3, &tmp_str3, 10);
        int arg4_int = strtol(arg4, &tmp_str4, 10);

        if(!strcmp(tmp_str1,"")){
            selection->row_start = arg1_int-1;
        }
        else{
            selection->row_start = 0;
        }

        if(!strcmp(tmp_str2,"")){
            selection->col_start = arg2_int-1;
        }
        else{
            selection->col_start = 0;
        }

        if(!strcmp(tmp_str3,"")){
            selection->row_end = arg3_int-1;
        }
        else if(!strcmp(tmp_str3,"-")){
            selection->row_end = table->row_cnt-1;
        }
        else{
            selection->row_start = 0;
        }

        if(!strcmp(tmp_str4,"")){
            selection->col_end = arg4_int-1;
        }
        else if(!strcmp(tmp_str4,"-")){
            selection->col_end = cells-1;
        }
        else{
            selection->row_start = 0;
        }
    }
}

//Vloží prázdný řádek před první vybraný řádek
void insert_row(selection_t *selection, table_t *table, int cells){
    table->row_cnt++;
    table->rows = realloc(table->rows, (table->row_cnt) * (sizeof(table_t)));
    for(int i = table->row_cnt-1; i > selection->row_start; i--){
        if(i == table->row_cnt-1){
            table->rows[i].cells = malloc(sizeof(row_t)*cells);
            table->rows[i].cell_cnt = cells;
        }
        for(int j = 0; j < cells; j++){
            table->rows[i].cells[j].cell = table->rows[i-1].cells[j].cell;
        }
    }
    for(int i = 0; i < cells; i++){
        table->rows[selection->row_start].cells[i].cell = "";
    }
}

//Přidá prázdný řádek za poslední vybraný řádek
void append_row(selection_t *selection, table_t *table, int cells){
    table->row_cnt++;
    table->rows = realloc(table->rows, (table->row_cnt + 1) * (sizeof(table_t)));
    for(int i = table->row_cnt-1; i > selection->row_end; i--){
        if(i == table->row_cnt-1){
            table->rows[i].cells = malloc(sizeof(row_t)*cells);
            table->rows[i].cell_cnt = cells;
        }
        for(int j = 0; j < cells; j++){
            table->rows[i].cells[j].cell = table->rows[i-1].cells[j].cell;
        }
    }
    for(int i = 0; i < cells; i++){
        table->rows[selection->row_end+1].cells[i].cell = "";
    }
}

//Přidá prázdný řádek na konec celé tabulky
void append_row_end(table_t *table, int cells){
    table->row_cnt++;
    table->rows = realloc(table->rows, (table->row_cnt + 1) * (sizeof(table_t)));
    table->rows[table->row_cnt-1].cells = malloc(sizeof(row_t)*cells);
    for(int i = 0; i < cells; i++){
        table->rows[table->row_cnt-1].cells[i].cell = "";
    }
    table->rows[table->row_cnt-1].cell_cnt = cells;
}

//Smaže všechny vybrané řádky
void delete_row(selection_t *selection, table_t *table, int cells){
    int deleted_rows = selection->row_end - selection->row_start + 1;
    if(table->row_cnt < 0){
        table->row_cnt = 0;
    }
    if(table->row_cnt != 0){
        for(int i = selection->row_end + 1; i < table->row_cnt; i++){
            for(int j = 0; j < cells; j++){
                table->rows[i-deleted_rows].cells[j].cell = table->rows[i].cells[j].cell;
            }
        }
        for(int i = 0; i < deleted_rows; i++){
            free(table->rows[table->row_cnt-i-1].cells);
        }
    }
    table->row_cnt = table->row_cnt-deleted_rows;
    
}

//Přidá prázdný sloupec před první vybraný sloupec
void insert_col(selection_t *selection, table_t *table, int *cells){
    (*cells)++;
    for(int i = 0; i < table->row_cnt; i++){
        table->rows[i].cell_cnt++;
        table->rows[i].cells = realloc(table->rows[i].cells, (table->rows[i].cell_cnt + 1) * (sizeof(row_t)));
        for(int j = *cells; j > selection->col_start; j--){
            table->rows[i].cells[j].cell = table->rows[i].cells[j-1].cell;
        }
        table->rows[i].cells[selection->col_start].cell = "";
    }
}

//Přidá prázdný sloupec za poslední vybraný sloupec
void append_col(selection_t *selection, table_t *table, int *cells){
    (*cells)++;
    for(int i = 0; i < table->row_cnt; i++){
        table->rows[i].cell_cnt++;
        table->rows[i].cells = realloc(table->rows[i].cells, (table->rows[i].cell_cnt + 1) * (sizeof(row_t)));
        for(int j = *cells; j > selection->col_end; j--){
            table->rows[i].cells[j].cell = table->rows[i].cells[j-1].cell;
        }
        table->rows[i].cells[selection->col_end+1].cell = "";
    }
}

//Přidá prázdný sloupec na konec celé tabulky
void append_col_end(table_t *table, int *cells){
    (*cells)++;
    for(int i = 0; i < table->row_cnt; i++){
        table->rows[i].cell_cnt++;
        table->rows[i].cells = realloc(table->rows[i].cells, (table->rows[i].cell_cnt + 1) * (sizeof(row_t)));
        table->rows[i].cells[*cells - 1].cell = "";
    }
}

//Smaže všechny vybrané sloupce
void delete_col(selection_t *selection, table_t *table, int *cells){
    int deleted_cols = 1;
    deleted_cols = selection->col_end - selection->col_start + 1;

    for(int i = 0; i < table->row_cnt; i++){
        for(int j = selection->col_end+1; j < *cells; j++){
            table->rows[i].cells[j-deleted_cols].cell = table->rows[i].cells[j].cell;
        }
        for(int j = 0; j < deleted_cols; j++){
            table->rows[i].cell_cnt--;
            //table->rows[i].cells[*cells - j - 1].cell = NULL;
            //free(table->rows[i].cells[*cells - j - 1].cell);
        }
    }
    (*cells) += -deleted_cols;
}

//Do všech vybraných buněk nastaví daný řetězec
void set_cell(selection_t *selection, table_t *table, char *current_arg){
    char *set_str = current_arg + 4;
    
    if((set_str[0] == '\"') && (set_str[strlen(set_str)-1] == '\"')){
        for(int i = 0; i < (int)strlen(set_str)-1; i++){
            set_str[i] = set_str[i+1];
        }
        set_str[strlen(set_str)-2] = '\0';
    } 
    else{
        char *escape_check;
        while((escape_check = strchr(set_str, '\\')) != NULL){
            for(int i = 0; i < (int)strlen(escape_check); i++){
                escape_check[i] = escape_check[i+1];
            }
        }
    }

    for(int i = selection->row_start; i <= selection->row_end; i++){
        for(int j = selection->col_start; j <= selection->col_end; j++){
            table->rows[i].cells[j].cell = set_str;
        }
    }
}

//Odstraní obsah vybraných buněk
void clear_cell(selection_t *selection, table_t *table){
    for(int i = selection->row_start; i <= selection->row_end; i++){
        for(int j = selection->col_start; j <= selection->col_end; j++){
            table->rows[i].cells[j].cell = "";
        }
    }
}

//Prohí všechny vybrané buňky s jednou cílovou buňkou
void swap_cell(selection_t *selection, table_t *table, char *current_arg){
    char *tmp_str = current_arg + 6;
    tmp_str[strlen(tmp_str)-1] = '\0';
    char *swap_row = strtok(tmp_str, ",");
    char *swap_col = strtok(NULL, ",");
    int swap_row_int = atoi(swap_row)-1;
    int swap_col_int = atoi(swap_col)-1;
    for(int i = selection->row_start; i <= selection->row_end; i++){
        for(int j = selection->col_start; j <= selection->col_end; j++){
            char *tmp_str_swap = table->rows[i].cells[j].cell;
            table->rows[i].cells[j].cell = table->rows[swap_row_int].cells[swap_col_int].cell;
            table->rows[swap_row_int].cells[swap_col_int].cell = tmp_str_swap;
        }
    }
}

//Sečte všechny čísla ve vybraných buňkách a uloží součet do zvolené buňky
void sum_cell(selection_t *selection, table_t *table, char *current_arg, int *cells){
    char *tmp_str = current_arg + 5;
    tmp_str[strlen(tmp_str)-1] = '\0';
    char *sum_row = strtok(tmp_str, ",");
    char *sum_col = strtok(NULL, ",");
    int sum_row_int = atoi(sum_row)-1;
    int sum_col_int = atoi(sum_col)-1;

    if((sum_row_int > table->row_cnt) || (sum_col_int > *cells - 1)){
        return;
    }

    double sum = 0.0;
    for(int i = selection->row_start; i <= selection->row_end; i++){
        for(int j = selection->col_start; j <= selection->col_end; j++){
            char *tmp_str2;
            double cell_num = strtod(table->rows[i].cells[j].cell, &tmp_str2);
            if(!strcmp(tmp_str2, "")){
                sum += cell_num;
            }
        }
    }
    char sum_str[50];
    sprintf(sum_str, "%g", sum);
    strcpy(table->rows[sum_row_int].cells[sum_col_int].cell, sum_str);
}

//Vypočítá průměr z čísel ve všech vybraných buňkách a uloží ho do zvolené buňky
void avg_cell(selection_t *selection, table_t *table, char *current_arg, int *cells){
    char *tmp_str = current_arg + 5;
    tmp_str[strlen(tmp_str)-1] = '\0';
    char *avg_row = strtok(tmp_str, ",");
    char *avg_col = strtok(NULL, ",");
    int avg_row_int = atoi(avg_row)-1;
    int avg_col_int = atoi(avg_col)-1;
    double sum = 0.0;
    int cells_counted = 0;

    if((avg_row_int > table->row_cnt) || (avg_col_int > *cells - 1)){
        return;
    }

    for(int i = selection->row_start; i <= selection->row_end; i++){
        for(int j = selection->col_start; j <= selection->col_end; j++){
            char *tmp_str2;
            double cell_num = strtod(table->rows[i].cells[j].cell, &tmp_str2);
            if(!strcmp(tmp_str2, "")){
                sum += cell_num;
                cells_counted++;
            }
        }
    }
    double avg = sum/cells_counted;
    char avg_str[50];
    sprintf(avg_str, "%g", avg);
    strcpy(table->rows[avg_row_int].cells[avg_col_int].cell, avg_str);
}

//Spočítá počet všech nbeprázdných buněk z vybraných buněk a uloží ho do zvolené buňky
void count_cell(selection_t *selection, table_t *table, char *current_arg, int *cells){
    char *tmp_str = current_arg + 7;
    tmp_str[strlen(tmp_str)-1] = '\0';
    char *count_row = strtok(tmp_str, ",");
    char *count_col = strtok(NULL, ",");
    int count_row_int = atoi(count_row)-1;
    int count_col_int = atoi(count_col)-1;
    int not_empty_cells = 0;

    if((count_row_int > table->row_cnt) || (count_col_int > *cells - 1)){
        return;
    }

    for(int i = selection->row_start; i <= selection->row_end; i++){
        for(int j = selection->col_start; j <= selection->col_end; j++){
            if(strcmp(table->rows[i].cells[j].cell, "")){
                not_empty_cells++;
            }
        }
    }
    char count_str[50];
    sprintf(count_str, "%d", not_empty_cells);
    strcpy(table->rows[count_row_int].cells[count_col_int].cell, count_str);
}

//Spočítá délku poslední vybrané buňky a uloží ji do zvolené buňky
void len_cell(selection_t *selection, table_t *table, char *current_arg, int *cells){
    char *tmp_str = current_arg + 5;
    tmp_str[strlen(tmp_str)-1] = '\0';
    char *len_row = strtok(tmp_str, ",");
    char *len_col = strtok(NULL, ",");
    int len_row_int = atoi(len_row)-1;
    int len_col_int = atoi(len_col)-1;
    
    if((len_row_int > table->row_cnt) || (len_col_int > *cells - 1)){
        return;
    }

    int len = strlen(table->rows[selection->row_end].cells[selection->col_end].cell);
    char len_str[50];
    sprintf(len_str, "%d", len);
    strcpy(table->rows[len_row_int].cells[len_col_int].cell, len_str);
}

//Do zvolené proměnná nastaví hodnotu z poslední vybrané buňky
void def_tmp_var(selection_t *selection,table_t *table, temp_vars_t *temp_vars, char *current_arg){
    char *tmp_str = current_arg + 5;
    int var_num = atoi(tmp_str);
    if((var_num >=0) && (var_num <=9)){
        temp_vars->vars[var_num] = table->rows[selection->row_end].cells[selection->col_end].cell;
    }
}

//Do vybraných buněk nastaví hodnotu ze zvolené dočasné proměnné
void use_tmp_var(selection_t *selection,table_t *table, temp_vars_t *temp_vars, char *current_arg){
    char *tmp_str = current_arg + 5;
    int var_num = atoi(tmp_str);
    if((var_num >=0) && (var_num <=9)){
        for(int i = selection->row_start; i <= selection->row_end; i++){
            for(int j = selection->col_start; j <= selection->col_end; j++){
                table->rows[i].cells[j].cell = temp_vars->vars[var_num];
            }
        }
    }
}

//Zvětší hodnotu v zvolené dočasné proměnné o 1, nebo ji nastaví na 1, pokud v ní nebylo číslo
void inc_tmp_var(temp_vars_t *temp_vars, char *current_arg){
    char *tmp_str = current_arg + 5;
    int var_num = atoi(tmp_str);
    if((var_num >=0) && (var_num <=9)){
        char *tmp_str2;
        int tmp_int = strtod(temp_vars->vars[var_num], &tmp_str2);
        if(!strcmp(tmp_str2, "")){
            tmp_int++;
        }
        else{
            tmp_int = 1;
        }
        sprintf(temp_vars->vars[var_num], "%d", tmp_int);
    }
}

//Uloží hodnoty z aktuální selekce do dočasně uložené selekce
void set_tmp_sel(selection_t *selection, temp_vars_t *temp_vars){
    temp_vars->sel.row_start = selection->row_start;
    temp_vars->sel.row_end = selection->row_end;
    temp_vars->sel.col_start = selection->col_start;
    temp_vars->sel.col_end = selection->col_end;
}

//Načte hodnoty z dočasně uložené selekce do aktuální selekce
void load_tmp_sel(selection_t *selection, temp_vars_t *temp_vars){
    selection->row_start = temp_vars->sel.row_start;
    selection->row_end = temp_vars->sel.row_end;
    selection->col_start = temp_vars->sel.col_start;
    selection->col_end = temp_vars->sel.col_end;
}

//Zkontroluje, jestli je v zadané dočasné proměnné 0 a pokud ano, tak vrátí o kolik příkazů se průchod posunout
int is_zero(temp_vars_t *temp_vars, char *current_arg){
    char *tmp_str = current_arg + 8;
    int var_int = tmp_str[0] - 48; //čísla 0-9 jsou v ascii tabůlce posunuty o 48
    char *increment = tmp_str + 2;
    int increment_int = atoi(increment);
    
    if((var_int >= 0) && (var_int <= 9)){
        char *tmp_var_check;
        int tmp_var_int = strtol(temp_vars->vars[var_int], &tmp_var_check, 10);
        if((!strcmp(tmp_var_check, "")) && (tmp_var_int == 0)){
            return increment_int;
        }
    }
    return 0;
}

//Od hodnoty v první zadné dočasné proměnné odečte hodnotu v druhé
void sub(temp_vars_t *temp_vars, char *current_arg){
    int temp_var_num1 = current_arg[5] - 48; //čísla 0-9 jsou v ascii tabůlce posunuty o 48
    int temp_var_num2 = current_arg[8] - 48;
    char *tmp_str1;
    char *tmp_str2;
    int tmp_var_int1 = strtol(temp_vars->vars[temp_var_num1], &tmp_str1, 10);
    int tmp_var_int2 = strtol(temp_vars->vars[temp_var_num2], &tmp_str2, 10);

    if((!strcmp(tmp_str1, "")) && (!strcmp(tmp_str2, ""))){
        int sub_sum = tmp_var_int1 - tmp_var_int2;
        sprintf(temp_vars->vars[temp_var_num1], "%d", sub_sum);
    }
}

//Základmí nastavení výběru na [1,1]
void selec_init(selection_t *selection){
    selection->row_start = 0;
    selection->row_end = 0;
    selection->col_start = 0;
    selection->col_end = 0;
}

//Základmí nastavení výběru na [1,1]
void temp_vars_init(temp_vars_t *temp_vars){
    temp_vars->sel.row_start = 0;
    temp_vars->sel.row_end = 0;
    temp_vars->sel.col_start = 0;
    temp_vars->sel.col_end = 0;
}

// Zarovná tabulku tak, aby všechny řádky měli stejný počet buněk
void align_table(table_t *table, int cells){
for(int i = 0; i < table->row_cnt; i++){
    for(;table->rows[i].cell_cnt < cells;){
        table->rows[i].cell_cnt++;
        table->rows[i].cells = realloc(table->rows[i].cells, (table->rows[i].cell_cnt) * (sizeof(row_t)));
        table->rows[i].cells[table->rows[i].cell_cnt-1].cell = "\0";
    }
}
}

//Finální zápis tabulky zpět do souboru
void final_write(table_t table, FILE *file, char delim){
    for(int i = 0; i < table.row_cnt-1; i++){
        for(int j = 0; j < table.rows[i].cell_cnt;j++){
            if(j == 0){
                if(strchr(table.rows[i].cells[j].cell, delim) != NULL){
                    fprintf(file, "\"%s\"", table.rows[i].cells[j].cell);
                }
                else{
                fprintf(file, "%s", table.rows[i].cells[j].cell);
                }
            }
            else{
                if(strchr(table.rows[i].cells[j].cell, delim) != NULL){
                    fprintf(file, "%c\"%s\"", delim, table.rows[i].cells[j].cell);
                }
                else{
                    fprintf(file, "%c%s", delim, table.rows[i].cells[j].cell);
                }
            }
        }
        if(i != table.row_cnt-1){
            fprintf(file, "\n");
        }
    }
}


int main(int argc, char *argv[]){
    char *delim;
    char *cmd_sequence;
    char *file_name;

    if(argc == 5){
        delim = argv[2];
        cmd_sequence = argv[3];
        file_name = argv[4];
    }
    else if(argc == 3){
        delim = " ";
        cmd_sequence = argv[1];
        file_name = argv[2];
    }
    else{
        fprintf(stderr, "You have used invalid sequence of arguments!");
        return 1;
    }

    FILE *file;
    file = fopen(file_name, "r");

    char CurrentChar = '\0';
    int cell_count = 0;

    table_t table;
    table.row_cnt = 0;
    //Kontrola délky souboru
    fseek(file, 0, SEEK_END);
    int file_size = 0;
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    if(file_size == 0){
        fclose(file);
        fprintf(stderr, "The file doesn't exist or is empty or something...");
        return 1;
    }


    char **tmp_str_read = malloc(file_size);
    int tmp_str_count = 0;


    //Nahrátí celého souboru do 2D tabulky
    while(CurrentChar != EOF){
        if(table.row_cnt == 0){
            table.row_cnt++;
            table.rows = malloc(sizeof(table_t));
        }
        else{
            table.row_cnt++;
            table.rows = realloc(table.rows, (table.row_cnt) * (sizeof(table_t)));
        }
        table.rows[table.row_cnt-1].cell_cnt = 0;
        while(CurrentChar != EOF){
            if(table.rows[table.row_cnt-1].cell_cnt == 0){
                table.rows[table.row_cnt-1].cell_cnt++;
                table.rows[table.row_cnt-1].cells = malloc(sizeof(row_t));
            }
            else{
                table.rows[table.row_cnt-1].cell_cnt++;
                table.rows[table.row_cnt-1].cells = realloc(table.rows[table.row_cnt-1].cells, (table.rows[table.row_cnt-1].cell_cnt) * (sizeof(row_t)));
            }

            table.rows[table.row_cnt-1].cells[table.rows[table.row_cnt-1].cell_cnt-1].cell_len = 0;
            int check_for_delim_quote = 1;
            int check_for_delim_escape = 1;

            int tmp_str_len = 0;
            
            tmp_str_read[tmp_str_count] = malloc(110);
            while(CurrentChar != EOF){
                CurrentChar = fgetc(file);
                int SaveChar = 1;
                if((CurrentChar == '\"') && (check_for_delim_escape == 1)){
                    if(check_for_delim_quote == 1){
                        check_for_delim_quote = 0;
                        CurrentChar = fgetc(file);
                    }
                    else{
                        check_for_delim_quote = 1;
                        SaveChar = 0;
                    }
                }
                if(check_for_delim_escape == 0){
                    check_for_delim_escape = 1;
                }
                if((CurrentChar == '\\') && (check_for_delim_quote == 1)){
                    CurrentChar = fgetc(file);
                    check_for_delim_escape = 0;
                }

                if((check_for_delim_quote == 1) && (check_for_delim_escape == 1)){
                    for(int i = 1; i < (int)strlen(delim); i++){
                        if(CurrentChar == delim[i]){
                            CurrentChar = delim[0];
                        }
                    }
                }
                if((tmp_str_len != 0) && (tmp_str_len%100 == 0)){
                    tmp_str_read[tmp_str_count] = realloc(tmp_str_read[tmp_str_count], tmp_str_len + 110);
                }
                if(SaveChar == 1){
                    tmp_str_read[tmp_str_count][tmp_str_len] = CurrentChar;
                    tmp_str_len++;
                }
                if((CurrentChar == delim[0]) && (check_for_delim_quote == 1) && (check_for_delim_escape == 1)){
                    break;
                }
                if(CurrentChar == '\n'){
                    break;
                }
            }
            
            tmp_str_read[tmp_str_count][tmp_str_len-1] = '\0';
            table.rows[table.row_cnt-1].cells[table.rows[table.row_cnt-1].cell_cnt-1].cell = tmp_str_read[tmp_str_count];
            tmp_str_count++;
            tmp_str_read = realloc(tmp_str_read, tmp_str_count*16);
            if(CurrentChar == '\n'){
                break;
            }
        }
        if(table.rows[table.row_cnt-1].cell_cnt > cell_count){
            cell_count = table.rows[table.row_cnt-1].cell_cnt;
        }
    }

    align_table(&table, cell_count);
    
    //Uložení všech příkazů do pole
    char **args = malloc(1000);
    char *current_arg;
    int arg_count = 0;
    current_arg = strtok(cmd_sequence, ";");
    while(current_arg != NULL){
        args[arg_count] = current_arg;
        arg_count++;
        current_arg = strtok(NULL, ";");
    }

    selection_t selection;
    selec_init(&selection);

    temp_vars_t temp_vars;
    temp_vars_init(&temp_vars);

    //Průchod všech příkazů a úprava tabulky podle toho
    for(int i = 0; i < arg_count; i++){
        current_arg = args[i];
        if((!strcmp(current_arg, "[min]"))){
            find_min(&selection, &table);
        }
        else if(!strcmp(current_arg, "[max]")){
            find_max(&selection, &table);
        }
        else if(!strncmp(current_arg, "[find ", 6)){
            find_str(&selection, &table, current_arg);
        }
        else if(!strcmp(current_arg, "irow")){
            insert_row(&selection, &table, cell_count);
        }
        else if(!strcmp(current_arg, "arow")){
            append_row(&selection, &table, cell_count);
        }        
        else if(!strcmp(current_arg, "drow")){
            delete_row(&selection, &table, cell_count);
        }
        else if(!strcmp(current_arg, "icol")){
            insert_col(&selection, &table, &cell_count);
        }
        else if(!strcmp(current_arg, "acol")){
            append_col(&selection, &table, &cell_count);
        }
        else if(!strcmp(current_arg, "dcol")){
            delete_col(&selection, &table, &cell_count);
        }
        else if(!strncmp(current_arg, "set ", 4)){
            set_cell(&selection, &table, current_arg);
        }
        else if(!strcmp(current_arg, "clear")){
            clear_cell(&selection, &table);
        }
        else if(!strncmp(current_arg, "swap [", 6)){
            swap_cell(&selection, &table, current_arg);
        }
        else if(!strncmp(current_arg, "sum [", 5)){
            sum_cell(&selection, &table, current_arg, &cell_count);
        }
        else if(!strncmp(current_arg, "avg [", 5)){
            avg_cell(&selection, &table, current_arg, &cell_count);
        }
        else if(!strncmp(current_arg, "count [", 7)){
            count_cell(&selection, &table, current_arg, &cell_count);
        }
        else if(!strncmp(current_arg, "len [", 5)){
            len_cell(&selection, &table, current_arg, &cell_count);
        }
        else if(!strncmp(current_arg, "def _", 5)){
            def_tmp_var(&selection, &table, &temp_vars, current_arg);
        }
        else if(!strncmp(current_arg, "use _", 5)){
            use_tmp_var(&selection, &table, &temp_vars, current_arg);
        }
        else if(!strncmp(current_arg, "inc _", 5)){
            inc_tmp_var(&temp_vars, current_arg);
        }
        else if(!strcmp(current_arg, "[set]")){
            set_tmp_sel(&selection, &temp_vars);
        }
        else if(!strcmp(current_arg, "[_]")){
            load_tmp_sel(&selection, &temp_vars);
        }
        else if(!strncmp(current_arg, "goto ", 5)){
            int goto_step = atoi(current_arg + 5);
            i += goto_step;
        }
        else if(!strncmp(current_arg, "iszero _", 8)){
            i += is_zero(&temp_vars, current_arg);
        }
        else if(!strncmp(current_arg, "sub _", 5)){
            sub(&temp_vars, current_arg);
        }
        else if((current_arg[0] == '[') && (current_arg[strlen(current_arg)-1] == ']')){
            normal_selection(&selection, &table, current_arg, cell_count);
            //doplnění prázdných řádků a sloupců na konec tabulky, když selekce přesahuje velikost tabulky
            while(selection.row_end >= table.row_cnt){
                append_row_end(&table, cell_count);
            }        
            while(selection.col_end >= cell_count){
                append_col_end(&table, &cell_count);
            }
        }
    }

    fclose(file);
    file = fopen(file_name, "w");
    final_write(table, file, delim[0]);

    for(int i = 0; i < table.row_cnt; i++){
        free(table.rows[i].cells);
    }
    for(int i = 0; i < tmp_str_count; i++){
        free(tmp_str_read[i]);
    }
    free(table.rows);
    free(args);
    free(tmp_str_read);
    fclose(file);

    return 0;
}