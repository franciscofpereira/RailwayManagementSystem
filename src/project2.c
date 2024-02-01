/* 
    * iaed-23 - ist1107502 - project1 
    * Francisco Ferro Pereira - LEIC-T
    * File: project1.c
    * Description: Implementation of a public transportation management system.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "project2.h"

int main(){

    char input[65535-1];
    
    /* initializes lists */
    ListOfRoutes* r_list = InitializeListOfRoutes();
    ListOfStops* s_list = InitializeListOfStops(); 
    
    while(1){
      
        /* reads command character */
        int ch = getchar(); 

        if (ch == 'q'){
            freesAllMemory(r_list,s_list);
            exit(0);
        }
        
        /* reads stdin into input string */
        if (fgets(input, 65535-1, stdin) == NULL) {
            exit(1);
        }

        switch(ch){
            
            case 'c':
                command_c(input, r_list, s_list);
                break;

            case 'p':
                command_p(input, r_list, s_list);
                break;

            case 'l':
                command_l(input, r_list, s_list);
                break;

            case 'i':
                command_i(r_list, s_list);
                break;
            
            case 'r':
                command_r(input, r_list, s_list);
                break;
            
            case 'e':
                command_e(input, r_list, s_list);
                break;
            
            case 'a':
                command_a(r_list, s_list);
                /* reinitialzies the lists */
                r_list = InitializeListOfRoutes();
                s_list = InitializeListOfStops();
                break;
            
            /* ignores white spaces */
            default:
                if (ch == ' ' || ch == '\t' || ch == '\n') break;
        }
    }
    return 0;
}


/******************************************************************************
                                   COMMANDS
******************************************************************************/

void command_c(char input[], ListOfRoutes* r_list, ListOfStops* s_list){
    listRoutes(input, r_list, s_list);
}

void command_p(char input[], ListOfRoutes* r_list, ListOfStops* s_list){
    listStops(input, s_list, r_list);
}

void command_l(char input[], ListOfRoutes* r_list, ListOfStops* s_list){
    AddLinks(input, r_list, s_list);
}

void command_i(ListOfRoutes* r_list, ListOfStops* s_list){
    listIntersections(r_list, s_list);
}


void command_r(char input[], ListOfRoutes* r_list, ListOfStops* s_list){
    removesRoute(input, r_list, s_list);
}

void command_e(char input[], ListOfRoutes* r_list, ListOfStops* s_list){
    removesStop(input, r_list, s_list);
}

void command_a(ListOfRoutes* r_list, ListOfStops* s_list){
    freesAllMemory(r_list, s_list);
}

/******************************************************************************
                                 AUX FUNCTIONS
******************************************************************************/


/* Reads input, performs tasks with 'c' command depending on num of args. */
void listRoutes(char input[], ListOfRoutes* r_list, ListOfStops* s_list){

    int n_args; 
    char *r_name = NULL;
    char inv_command[8];
    
    /* allocates memory to store s_name */
    r_name = malloc(strlen(input) + 1);
    validateMalloc(r_name, r_list, s_list);

    /* if quotes found, uses format specifier to parse names between quotes */
    if(QuotesFound(input)){
        n_args = sscanf(input, " \"%[^\"]\" %s", r_name, inv_command);
    }
    else{
        n_args = sscanf(input, "%s %s", r_name, inv_command);
    }   
    /* if there are no arguments in the input string */
    if (n_args == -1){
        listRoutesNoArguments(r_list);            
    }    
    else if (n_args == 1){     
        listRoutesOneArgument(r_name, r_list, s_list);    
    }
    else if (n_args== 2){
        listRoutesTwoArguments(r_name, inv_command, r_list);   
    }
    
    free(r_name);
}


/* Reads input, executes 'p' command tasks depending on num of args. */
void listStops(char input[], ListOfStops* s_list, ListOfRoutes* r_list){

    int n_args;
    char* s_name = NULL;
    double s_lat, s_lon;
    stop* currStop;
    
    /* allocates memory to store s_name */
    s_name = malloc(strlen(input) + 1);
    validateMalloc(s_name, r_list, s_list);

    if(QuotesFound(input)){
        n_args = sscanf(input," \"%[^\"]\" %lf %lf", s_name, &s_lat, &s_lon);
    }

    else{
        n_args = sscanf(input, "%s %lf %lf", s_name, &s_lat, &s_lon);    
    }

    if( n_args == -1){

        currStop = s_list->head;

        /* iterates through stops, prints name, lat, lon and n_routes */
        while (currStop != NULL) {
            printf("%s: ", currStop->name);
            printf("%16.12f ", currStop->lat);
            printf("%16.12f ", currStop->lon);
            printf("%d\n", currStop->n_routes);
            currStop = currStop->next;           
        }
    }
    
    else if( n_args == 1){
          
        /* Latitude and longitude are printed if stop exists*/
        if(stopExists(s_list, s_name)){
            currStop = getStop(s_list, s_name); 
            printf("%16.12f ", currStop->lat);
            printf("%16.12f\n", currStop->lon);
        }
        else{
            printf("%s: no such stop.\n", s_name);
        }
    }
    else if( n_args == 3){

        /* error if user tries to create new stop with existing name */
        if (stopExists(s_list, s_name)){
            printf("%s: stop already exists.\n",s_name);
        }
        
        /* creates new stop if s_name isn't taken */
        else{ 
            createStop(s_list, r_list, s_name, s_lat, s_lon);
        }
    }
    
    free(s_name);
}



/* Reads input string and adds links to routes. */
void AddLinks(char input[], ListOfRoutes* r_list, ListOfStops* s_list){
    
    int n_args; 
    char* r_name = NULL;
    char* link_orig = NULL;
    char* link_dest = NULL;
    double cost, duration;
    
    /* allocates memory for r_name, link_orig and link_dest */
    r_name = malloc(strlen(input)+1);
    validateMalloc(r_name, r_list, s_list);

    link_orig = malloc(strlen(input)+1);
    validateMalloc(link_orig, r_list, s_list);

    link_dest = malloc(strlen(input)+1);
    validateMalloc(link_dest, r_list, s_list);
    
    if( QuotesFound(input)){

        /* link_orig is in quotes */
        if( (n_args = sscanf(input, "%s \"%[^\"]\" %s %lf %lf", r_name, 
        link_orig, link_dest, &cost, &duration)) && n_args == 5){}

        /* link_dest is in quotes */
        else if( (n_args = sscanf(input, "%s %s \"%[^\"]\" %lf %lf", r_name, 
        link_orig, link_dest, &cost, &duration)) && n_args == 5){}

        /* both link stops are in quotes */
        else if( (n_args = sscanf(input, "%s \"%[^\"]\" \"%[^\"]\" %lf %lf", 
        r_name, link_orig, link_dest, &cost, &duration)) && n_args == 5){}
    }

    /* there are no quotes in the input string */
    else{
        n_args = sscanf(input, "%s %s %s %lf %lf", r_name, link_orig, 
        link_dest, &cost, &duration);
    }
    
    /* if validation of link args fails, function ends execution with error */
    if(!validateLinks(r_name, link_orig, link_dest, cost, duration, 
                     r_list, s_list)){
        
        free(r_name), free(link_orig), free(link_dest);
        return;
    }
    
    /* if route is empty, adds both stops to route */
    if(routeIsEmpty(r_list, r_name)){
        addsStopsToEmptyRoute(r_name, link_orig, link_dest, cost, duration, 
                            r_list, s_list);
        
        free(r_name), free(link_orig), free(link_dest);
        return;
    }
    
    /*if link_orig is route's dest stop, link_dest is added to end of route. */
    else if(stopIsRouteDestination(r_list, r_name, link_orig)){
        
        addStopToRouteDest(r_name, link_dest, cost, duration, r_list, s_list);

        free(r_name), free(link_orig), free(link_dest);
        return;
    }
    
    /* if link_dest is route's origin stop, it's added to start of route. */
    else if(stopIsRouteOrigin(r_list, r_name, link_dest)){

        addStopToRouteOrig(r_name, link_orig, cost, duration, r_list, s_list);
        free(r_name), free(link_orig), free(link_dest);
        return;
    }

    free(r_name);
    free(link_orig);
    free(link_dest); 
}  



/* Function that checks if given route exists. */ 
int routeExists(ListOfRoutes* list, char* r_name){

    route* temp = list->head;

    while(temp != NULL){
        if(strcmp(r_name, temp->name) == 0){
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}


/* Function that checks if given stop exists. */
int stopExists(ListOfStops* list, char* s_name){
    
    stop* temp = list->head;

    while(temp != NULL){
        if(strcmp(s_name, temp->name) == 0){
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}


/* Creates route with given name. Returns pointer to head of ListOfRoutes */
route* createRoute(ListOfRoutes* r_list, ListOfStops* s_list, char* r_name){ 
    
    /* allocates memory for new instance of route */
    route* new_route = malloc(sizeof(route));
    validateMalloc(new_route, r_list, s_list);

    new_route->name = NULL;
    new_route->name = malloc(strlen(r_name) + 1);
    validateMalloc(new_route->name, r_list, s_list);

    if(r_name!= NULL){
        strcpy(new_route->name, r_name);
    }
    
    /* creates and initializes a RoutePath associated with route */
    new_route->path = createRoutePath(r_list, s_list);

    /* initializes next and prev pointers */
    new_route->next = NULL;
    new_route->prev = NULL;

    new_route->n_stops = 0;
    new_route->total_cost = 0;
    new_route->total_duration = 0;
    
    /* adds new_route to RouteList */
    addRouteToList(r_list, new_route);
    
    /* returns pointer to head of ListOfRoutes */
    return r_list->head ;
}




/* Creates stop with name, lat and lon. Returns pointer to ListOfStops head. */
stop* createStop(ListOfStops* s_list, ListOfRoutes* r_list, char* s_name, 
                double s_lat, double s_lon){
    
    /* allocates memory for a new stop and its name */
    stop* new_stop = malloc(sizeof(stop));
    validateMalloc(new_stop, r_list, s_list);
    
    new_stop->name = NULL;
    new_stop->name = malloc(strlen(s_name) + 1);
    validateMalloc(new_stop->name, r_list, s_list);

    if(s_name!=NULL){
        strcpy(new_stop->name, s_name);
    }
    
    new_stop->lat = s_lat;
    new_stop->lon = s_lon;
    new_stop->n_routes = 0;
    new_stop->prev = NULL;
    new_stop->next = NULL;
    new_stop->routesAtStop = NULL;

    addStopToList(s_list, new_stop);

    /* returns pointer to head of ListOfStops */
    return s_list->head;
}




/* Retrieves the route struct from ListOfRoutes given its name. */
route* getRoute(ListOfRoutes* list, char* r_name){
    
    route* temp = list->head;
    
    while(temp != NULL){
        if(strcmp(r_name, temp->name) == 0){
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}



/* Retrieves the stop struct from ListOfStops given its name. */
stop* getStop(ListOfStops* list, char* s_name){
    
    stop* temp = list->head;

    while(temp != NULL){
        if(strcmp(s_name, temp->name) == 0){
            return temp;
        }
        temp = temp->next;
    }
    return NULL;    
}



/* Function that checks if input string contains quotes. */
int QuotesFound(const char input[]) {
    int i;
    
    for (i = 0; input[i] != '\0'; i++) {
        if (input[i] == '\"') {
            return 1;
        }
    }
    return 0;
}


/* Function that checks if 'inverso' command is valid. */
int inverseIsValid(char inv_command[]) {
    
    int i, len_inv_command = strlen(inv_command);

    if (strlen(inv_command) >= 3 && strlen(inv_command) <= 7) {
        char valid_command[] = "inverso";
        for (i = 0; i < len_inv_command; i++) {
            if (inv_command[i] != valid_command[i]) {
                return 0;
            }
        }
        return 1;      
    }
    else{
        return 0;
    }
}



/* Function that validates the links arguments. */
int validateLinks(char* r_name, char link_orig[], char link_dest[], 
                 double cost, double duration, 
                 ListOfRoutes* r_list,ListOfStops* s_list){
    
    /* prints error message if the specified route does not exist */
    if( !routeExists(r_list, r_name)){
        printf("%s: no such line.\n", r_name);
        return 0;
    }
    
    /* prints error message if link orig stop does not exist */
    else if( !stopExists(s_list, link_orig)){
        printf("%s: no such stop.\n", link_orig);
        return 0;
    }

    /* prints error message if link dest stop does not exist*/
    else if( !stopExists(s_list,link_dest)){
        printf("%s: no such stop.\n", link_dest);
        return 0;
    }
    
    /* error if link orig is not in route and link dest is not route orig */
    else if(!routeIsEmpty(r_list,r_name) && 
    !stopIsInRoute(r_list, r_name,link_orig) && 
    !stopIsRouteOrigin(r_list,r_name,link_dest)){
        printf("link cannot be associated with bus line.\n");
        return 0;
    }
    
    /* error if link dest is not in route and link orig is not route dest */
    else if( !routeIsEmpty(r_list,r_name) && 
    !stopIsInRoute(r_list, r_name,link_dest) && 
    !stopIsRouteDestination(r_list,r_name,link_orig)){
        printf("link cannot be associated with bus line.\n");
        return 0;
    }

    /* error if link dest is not route orig and link orig is not route dest */
    else if( !routeIsEmpty(r_list,r_name) && 
    !stopIsRouteOrigin(r_list, r_name, link_dest) && 
    !stopIsRouteDestination(r_list, r_name, link_orig)){
        printf("link cannot be associated with bus line.\n");
        return 0;
    }
    /* prints error message if the cost or duration are negative */
    else if( cost<0 || duration<0){
        printf("negative cost or duration.\n");
        return 0;
    }
    else{
        return 1;
    }
}



/* Function that adds link stops to an empty route. */
void addsStopsToEmptyRoute(char* r_name, char link_orig[], char link_dest[], 
                          double cost, double duration, ListOfRoutes *r_list,
                          ListOfStops *s_list){

    route* currRoute ;
    stop* origStop, *destStop;
    RoutePath* path;
    RoutePathNode* origNode, *destNode;

    /* gets route */
    currRoute = getRoute(r_list, r_name);
    if (currRoute == NULL) return;
        
    path = currRoute->path;

    /* gets origin and destination stops */
    origStop = getStop(s_list, link_orig);
    destStop = getStop(s_list, link_dest);

    /* if origStop is not in route, increments number of routes with origStop*/
    if(!stopIsInRoute(r_list, r_name, origStop->name) 
                        && !nameIsInArray(r_name, origStop)){
        ++origStop->n_routes;

        /* adds route name to array of routes that intersect stop */
        addsNameToRoutesAtStop(&(origStop->routesAtStop), origStop->n_routes, 
                                r_name, r_list, s_list);
    }
    

   /* if destStop is not in route, increments number of routes with destStop */
    if(!stopIsInRoute(r_list, r_name, destStop->name) 
                        && !nameIsInArray(r_name, destStop)){
        
        ++destStop->n_routes;

        /* adds route name to array of routes that intersect stop */
        addsNameToRoutesAtStop(&(destStop->routesAtStop), destStop->n_routes, 
                            r_name, r_list, s_list);   
    }

    /* allocates memory for new RoutePath nodes */
    origNode = malloc(sizeof(RoutePathNode));
    validateMalloc(origNode, r_list, s_list);
    destNode = malloc(sizeof(RoutePathNode)); 
    validateMalloc(destNode, r_list, s_list);
    
    /* initializes origNode and destNode pointers */
    origNode->Stop = origStop;
    origNode->next = NULL;
    origNode->prev = NULL;
    origNode->cost = cost;
    origNode->duration = duration;
    
    destNode->Stop = destStop;
    destNode->next = NULL;
    destNode->prev = NULL;
    destNode->cost = 0;
    destNode->duration = 0;

    /* assigns path nodes to RoutePath orig and dest */
    path->orig = origNode;
    path->dest = destNode;

    /* updates next and previous pointers */
    origNode->next = destNode;
    origNode->prev = NULL;
    destNode->prev = origNode;
    destNode->next = NULL;

    /* updates route n_stops, total_cost and total_duration */
    currRoute->n_stops += 2;
    currRoute->total_cost += cost;
    currRoute->total_duration += duration;   
}


/* Function that adds link to origin of the route. */
void addStopToRouteOrig(char* r_name, char* s_name, double cost, 
                double duration, ListOfRoutes *r_list, ListOfStops *s_list){

    route* currRoute;
    stop* currStop;
    RoutePath* path;
    RoutePathNode* newNode;

    /* gets the stop to be added */
    currStop = getStop(s_list, s_name);

    /* increments num of routes that intersect stop if not already in stop */
    if(!stopIsInRoute(r_list, r_name, s_name) 
                    && !nameIsInArray(r_name, currStop)){
        ++currStop->n_routes;
        addsNameToRoutesAtStop(&(currStop->routesAtStop), currStop->n_routes, 
                            r_name, r_list, s_list);  
    }

    /* gets route info */
    currRoute = getRoute(r_list, r_name);
    path = currRoute->path;

    newNode = malloc(sizeof(RoutePathNode));
    validateMalloc(newNode, r_list, s_list);

    /* initializes newNode fields */
    newNode->Stop = currStop;
    newNode->next = path->orig;
    newNode->prev = NULL;
    newNode->cost = cost; 
    newNode->duration = duration;
    
    /* assigns current orig prev pointer to newNode */
    path->orig->prev = newNode;

    /* newNode is now the path origin */
    path->orig = newNode;

    /* updates route n_stops, total_cost and total_duration */
    ++currRoute->n_stops;
    currRoute->total_cost += cost;
    currRoute->total_duration += duration;
}



/* Adds link to end of the route. */
void addStopToRouteDest(char* r_name, char* s_name, double cost, 
                double duration, ListOfRoutes *r_list, ListOfStops *s_list){
    
    route* currRoute;
    stop* currStop;
    RoutePath* path;
    RoutePathNode* newNode;

    /* gets the stop to be added */
    currStop = getStop(s_list, s_name);

    /* if stop is in route, increments number of routes that intersect it */
    if(!stopIsInRoute(r_list, r_name, s_name) 
                    && !nameIsInArray(r_name,currStop)){
        ++currStop->n_routes;
        
        /* adds route name to array of routes names that intersect currStop*/
        addsNameToRoutesAtStop(&(currStop->routesAtStop), currStop->n_routes, 
                                r_name, r_list, s_list);
    }

    /* gets route info */
    currRoute = getRoute(r_list, r_name);
    path = currRoute->path;

    /* allocates memory for newNode of RoutePath */
    newNode = malloc(sizeof(RoutePathNode));
    validateMalloc(newNode, r_list, s_list);

    /* initializes newNode fields */
    newNode->Stop = currStop;
    newNode->next = NULL;
    newNode->prev = path->dest;
    newNode->cost=  0;
    newNode->duration = 0;

    /* assigns current dest next pointer to newNode */
    path->dest->next = newNode;

    /* newNode is now the path destination */
    path->dest = newNode;

    /* the previous node contains the link cost and duration */
    newNode->prev->cost = cost; 
    newNode->prev->duration = duration;

    /* updates route n_stops, total_cost and total_duration */
    ++currRoute->n_stops;
    currRoute->total_cost += cost;
    currRoute->total_duration += duration;
}


 
/* Checks if stop is route destination. */
int stopIsRouteDestination(ListOfRoutes* r_list,char* r_name,char* s_name){

    route* currRoute;

    currRoute = getRoute(r_list, r_name);

    /* compares stop_name with routePath destination name */
    if( strcmp(currRoute->path->dest->Stop->name, s_name) == 0 ){
        return 1;
    }
    else{
        return 0;
    }
}


 
/* Checks if stop is route origin. */
int stopIsRouteOrigin(ListOfRoutes* r_list, char* r_name, char* s_name){
    
    route* currRoute;

    currRoute = getRoute(r_list, r_name);
   
    /* compares stop_name with route origin name*/
    if (strcmp(currRoute->path->orig->Stop->name, s_name) == 0) {
        return 1;
    }
    else {
        return 0;
    }
}


 
/* Checks if route is empty. */
int routeIsEmpty(ListOfRoutes* r_list, char* r_name){

    route* currRoute;

    currRoute = getRoute(r_list, r_name);

    if(currRoute->n_stops == 0){
        return 1;
    }
    else{
        return 0;
    }
}


/* Checks if given stop is in given route. */
int stopIsInRoute(ListOfRoutes* r_list, char* r_name, char* s_name){

    RoutePathNode* temp; 
    route* currRoute;
    currRoute = getRoute(r_list, r_name);
    
    temp = currRoute->path->orig;

    /* iterates through routePathNodes */
    while( temp != NULL ){
        
        if(strcmp(temp->Stop->name, s_name) == 0){
            return 1;
        }
        temp = temp->next;
    }
    return 0;  
}



/* Lists route intersections. */
void listIntersections(ListOfRoutes* r_list, ListOfStops* s_list){

    int i,j;
    stop* currStop = s_list->head;
    char** routesAtStopCopy;
    
    while(currStop!=NULL){

        /* prints name of stop and number of routes that intersect it */
        if(currStop->n_routes > 1){
            printf("%s %d: ", currStop->name, currStop->n_routes);
            
            /* creates a copy of RoutesAtStop */
            routesAtStopCopy = malloc(currStop->n_routes*sizeof(char*));
            validateMalloc(routesAtStopCopy, r_list, s_list);

            for (i = 0; i < currStop->n_routes; i++) {
                routesAtStopCopy[i] = currStop->routesAtStop[i];
            }
            
            /* sorts the copy of the array alphabetically*/
            insertionSortRouteNames(routesAtStopCopy, currStop->n_routes, 
                                    r_list, s_list);
            
            /* prints sorted route names */
            for(j=0; j< currStop->n_routes; j++){
                printf("%s", routesAtStopCopy[j]);

                /* prints a space after all route names except the last one */
                if(j< currStop->n_routes -1){
                    printf(" ");
                }
            }
            printf("\n");

            /* frees copy of the array */
            free(routesAtStopCopy);
        }
        currStop = currStop->next;
    }       
}
        

/* Sorts alphabetically the names of routes that intersect at a given stop. */   
void insertionSortRouteNames(char** routesAtStop, int size, 
                    ListOfRoutes* r_list, ListOfStops* s_list){
    
    int i, j, max_len = findBiggestRouteName(routesAtStop, size);
    char* key;

    /* allocates memory for key with enough space to hold the largest name */
    key = malloc((max_len+1)*sizeof(char));
    validateMalloc(key, r_list, s_list);

    for (i = 1; i < size; i++) {
        strcpy(key, routesAtStop[i]);
        j = i - 1;

        while (j >= 0 && strcmp(routesAtStop[j], key) > 0) {
            strcpy(routesAtStop[j + 1], routesAtStop[j]);
            j--;
        }
        strcpy(routesAtStop[j + 1], key);
    }

    free(key);
}

    

/* Checks if name of route is in array of routes that intersect a given stop.*/
int nameIsInArray(char* r_name, stop* currStop){
    
    int i;

    if(currStop->n_routes == 0){
        return 0;
    }

    for(i = 0; i < currStop->n_routes; i++){
        if(strcmp(r_name, currStop->routesAtStop[i]) == 0){
            return 1;
        }
    }
    return 0;
}



void listRoutesNoArguments(ListOfRoutes* r_list){
    
    route* currRoute;
    
    currRoute = r_list->head;

    while(currRoute != NULL){

        /* if currRoute has links */
        if(currRoute->n_stops >=2 ){
            printf("%s ", currRoute->name);
            printf("%s ", currRoute->path->orig->Stop->name);
            printf("%s ", currRoute->path->dest->Stop->name);
            printf("%d ", currRoute->n_stops);
            printf("%.2f ", currRoute->total_cost);
            printf("%.2f\n", currRoute->total_duration);
        }

        else{
            printf("%s ", currRoute->name);
            printf("%d ", currRoute->n_stops);
            printf("%.2f ", currRoute->total_cost);
            printf("%.2f\n", currRoute->total_duration);
        }

        currRoute = currRoute->next;
    }
}


void listRoutesOneArgument(char* r_name, ListOfRoutes* r_list, 
                            ListOfStops* s_list){
    route* currRoute;
    RoutePathNode* currNode;
    
    /* checks if route exists */
    if(routeExists(r_list, r_name)){
        
        currRoute = getRoute(r_list, r_name);
        if(currRoute == NULL) return;

        /* if the route has no stops execution of function ends */
        if(currRoute->n_stops == 0){
            return;
        }
        
        /* sets currStop to orig route stop */
        currNode = currRoute->path->orig;
        
        /* prints route stops names */
        while (currNode != NULL) {
            
            printf("%s", currNode->Stop->name);
    
            if(currNode->next != NULL){
                printf(", ");
            }
            currNode = currNode->next;
        }
        printf("\n");        
    }   
    /* if route does not exist, new one is added to ListOfRoutes */
    else{
        createRoute(r_list, s_list, r_name);    
    }
}


void listRoutesTwoArguments(char* r_name, char inv_command[], 
                            ListOfRoutes* list_routes){
    
    route* currRoute;
    RoutePathNode* currNode;
    
    /* checks if the route exists */
        if(routeExists(list_routes, r_name)){

            currRoute = getRoute(list_routes, r_name);
            
            /* checks if the inverse command is valid */
            if( inverseIsValid(inv_command)){

                /* if route has no stops, execution of function ends */
                if(currRoute->n_stops == 0){
                    return;
                }
                
                currNode = currRoute->path->dest;
                
                /* prints route stops in inverse order */
                while(currNode != NULL){
                    printf("%s", currNode->Stop->name);

                    if(currNode != currRoute->path->orig){
                        printf(", ");
                    }

                    currNode = currNode->prev;
                }
                printf("\n");   
            }
                 
            else{
                printf("incorrect sort option.\n");
            }   
        }
}


/*Validates memory allocation. Frees all memory and stops execution if failed*/
int validateMalloc(void* ptr, ListOfRoutes* r_list, ListOfStops* s_list){
    if (ptr == NULL) {
        printf("No memory.\n");
        freesAllMemory(r_list,s_list);
        exit(0);
    } 
    return 1;
}


ListOfRoutes* InitializeListOfRoutes(){
    ListOfRoutes* r_list = malloc(sizeof(ListOfRoutes));
    if(r_list == NULL){
        exit(0);
    }
    r_list->head = NULL;
    r_list->tail = NULL;
    return r_list;
}


ListOfStops* InitializeListOfStops(){
    
    ListOfStops* s_list = malloc(sizeof(ListOfStops));
    
    if(s_list == NULL){
        exit(0);
    }
    s_list->head = NULL;
    s_list->tail = NULL;
    return s_list;
}

RoutePathNode* InitializeRoutePathNode(RoutePathNode* node, stop* stop){
    node->Stop = stop;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

/* Creates a new instance of RoutePath and initializes its fields */
RoutePath* createRoutePath(ListOfRoutes* r_list, ListOfStops* s_list){
    RoutePath* path = malloc(sizeof(RoutePath));
    validateMalloc(path,r_list, s_list);

    path->orig = NULL;
    path->dest = NULL;

    return path;
}


/* Adds newly created route to ListOfRoutes */
void addRouteToList(ListOfRoutes* list, route* new_route){
    
    /* adds new route to head of ListOfRoutes if empty*/
    if(list->head == NULL){
        list->head = list->tail = new_route;
        new_route->prev = NULL;
    }
    /* adds to tail of ListOfRoutes */
    else{
        new_route->prev = list->tail;
        list->tail->next = new_route;
        list->tail = new_route;
    }
}

/* Adds newly created stop to ListOfStops */
void addStopToList(ListOfStops* list, stop* new_stop){
    
    /* adds new stop to head of ListOfStops if empty */
    if(list->head == NULL){
        list->head = list->tail = new_stop;
        new_stop->prev = NULL;
    }
    /* adds to tail of ListOfStops */
    else{
        new_stop->prev = list->tail;
        list->tail->next = new_stop;
        list->tail = new_stop;
    }
}


void addsNameToRoutesAtStop(char*** routesAtStop, int n_routes, char* r_name,
                            ListOfRoutes*r_list, ListOfStops* s_list){
    
    char** newRoutesAtStop;
    char* route_name;

    /* Reallocates memory for new route name (array size is already updated) */
    newRoutesAtStop = realloc(*routesAtStop, n_routes * sizeof(char*));
    validateMalloc(newRoutesAtStop, r_list, s_list);

    /* Allocates memory block for new route name and copies the name into it */
    route_name = malloc((strlen(r_name) + 1 ) * sizeof(char));
    validateMalloc(route_name, r_list, s_list);
    strcpy(route_name, r_name);

    /* Adds pointer to new route name to the end of the array*/
    newRoutesAtStop[n_routes - 1] = route_name;
    
    /* Updates the routesAtStop field of the stop struct */
    *routesAtStop = newRoutesAtStop;
}


int findBiggestRouteName(char** routesAtStop, int size){
    
    int len, i, max_len = 0;
    
    for(i=0; i<size; i++){
        len = strlen(routesAtStop[i]);
        if(len>max_len){
            max_len = len;
        }
    }
    return max_len;
}


/* Takes a pointer to RoutePath and frees all allocated memory for its nodes*/
void freesRoutePathNodes(RoutePath* path) {
    
    RoutePathNode* currNode = path->orig;
    RoutePathNode* nextNode;

    /* iterates through RoutePath and frees all RoutePathNodes */
    while (currNode != NULL) {
        nextNode = currNode->next;
        if (currNode == path->orig) {
            path->orig = NULL;
        }
        if (currNode == path->dest) {
            path->dest = NULL;
        }
        free(currNode);
        currNode = nextNode;
    }
}


/* Takes a pointer to Route path and frees all nodes and the path itself */
void freesRoutePath(RoutePath* path){
    
    /* frees all of the nodes inside path and then frees the path itself*/
    freesRoutePathNodes(path);
    free(path);
} 

/* Takes a pointer to ListOfRoutes and Frees all routes and the list itself */
void freesAllRoutes(ListOfRoutes* r_list){

    route* currRoute = r_list->head;
    route* nextRoute;

    /* iterates through ListOfRoutes and frees each route */
    while(currRoute != NULL){
        nextRoute = currRoute->next;
        free(currRoute->name);
        freesRoutePath(currRoute->path);
        free(currRoute);
        currRoute =  nextRoute;
    }
    
    /* frees ListOfRoutes struct */
    r_list->head = NULL;
    r_list->tail = NULL;
    free(r_list);
    r_list = NULL;
}


/*Frees all of names of routes that intersect at a stop and the array itself*/
void freeRoutesAtStopArray(char** routesAtStop, int n_routes) {
    
    int i;
    /* iterates though the array and frees each name */
    for (i = 0; i < n_routes; i++) {
        free(routesAtStop[i]);
    }
    /* frees the array itself */
    free(routesAtStop);
}


/* Takes a pointer to ListOfStops and frees all of the stops and the list */
void freesAllStops(ListOfStops* s_list){
    stop* currStop = s_list->head;
    stop* nextStop;

    while(currStop!= NULL){
        nextStop = currStop->next;
        free(currStop->name);
        freeRoutesAtStopArray(currStop->routesAtStop, currStop->n_routes);
        free(currStop);
        currStop = nextStop;
    }
    free(s_list);
}


/* Frees all dynamically allocated memory */
void freesAllMemory(ListOfRoutes* r_list, ListOfStops* s_list){

    freesAllRoutes(r_list);
    freesAllStops(s_list);
    r_list = NULL;
    s_list = NULL;
}


/* Removes a route and all its links */
void removesRoute(char input[], ListOfRoutes* r_list, ListOfStops* s_list){

    char* r_name;
    route* currRoute;
    stop* currStop;
    
    r_name = malloc(strlen(input) + 1);
    validateMalloc(r_name, r_list, s_list);
    
    sscanf(input, "%s", r_name);

    if(!routeExists(r_list, r_name)){
        printf("%s: no such line.", r_name);
        free(r_name);
        return;
    }
    
    currRoute = getRoute(r_list,r_name);

    /* updates all routesAtStop arrays and n_routes of each stop */
    currStop = s_list->head;
    while(currStop!=NULL){
        removeRouteNameFromArray(r_name, currStop);
        currStop = currStop->next;
    }
    
    /* removes currRoute from head of ListOfRoutes*/
    if(currRoute == r_list->head){
        r_list->head = currRoute->next;
        if (currRoute->next != NULL) {
            currRoute->next->prev = NULL;
        }
        free(currRoute->name);
        freesRoutePath(currRoute->path);
        free(currRoute);
    }

    /* removes currRoute from middle or tail of ListRoutes*/
    else{
        currRoute->prev->next = currRoute->next;
        currRoute->next->prev = currRoute->prev;
        free(currRoute->name);
        freesRoutePath(currRoute->path);
        free(currRoute);
    }
    
    free(r_name);
}


void removeRouteNameFromArray(char* r_name, stop* s) {
    
    int i, found = 0;
    
    /* iterates through routesAtStop */
    for (i = 0; i < s->n_routes; i++) {
        if (strcmp(s->routesAtStop[i], r_name) == 0) {
            found = 1;
            
            /* if its the last name in the array, removes the it from array */
            if (i == s->n_routes - 1) {
                free(s->routesAtStop[i]);
                s->n_routes--;
                s->routesAtStop = realloc(s->routesAtStop, 
                                    s->n_routes * sizeof(char*));
            
            /* removes the name and shifts all elements back in the array */
            } else {
                int j;
                for (j = i; j < s->n_routes - 1; j++) {
                    s->routesAtStop[j] = s->routesAtStop[j+1];
                }
                free(s->routesAtStop[s->n_routes - 1]);
                s->n_routes--;
                s->routesAtStop = realloc(s->routesAtStop, 
                                s->n_routes * sizeof(char*));
            }
            break;
        }
    }
    if (!found) {
        return;
    }
}


stop* removesStop(char input[], ListOfRoutes* r_list, ListOfStops* s_list){

    char* s_name;
    route* currRoute, *nextRoute;
    stop* currStop, *nextStop, *headStop = s_list->head;
    RoutePathNode* currNode, *nextNode;

    /* allocates memory for s_name */
    s_name = malloc(strlen(input) + 1);
    validateMalloc(s_name, r_list, s_list);

    sscanf(input, "%s", s_name);

    /* if the stop does not exist, ends function execution */
    if(!stopExists(s_list, s_name)){
        printf("%s: no such stop.\n", s_name);
        free(s_name);
        return headStop;
    }

    currStop = getStop(s_list, s_name);
    
    currRoute = r_list->head;
    nextRoute = currRoute->next;
    currNode = currRoute->path->orig;
    nextNode = currNode->next;
    /* removes the stop from all routes*/
    removesStopFromAllRoutes(s_name, currRoute, nextRoute, currNode, nextNode,
                            r_list, s_list);

    
    /* Iterates through ListOfStops and removes the stop from the list */
    currStop = s_list->head;
    while(currStop!=NULL){
        
        if(strcmp(currStop->name, s_name) == 0){
            
            if(currStop == s_list->head){
                nextStop = currStop->next;
                if (nextStop != NULL) {
                    nextStop->prev = NULL;
                }
                s_list->head = nextStop;
                free(currStop->name);
                freeRoutesAtStopArray(currStop->routesAtStop, 
                                    currStop->n_routes);
                free(currStop);
                free(s_name);
                return headStop;      
            }
        
            else{
                nextStop = currStop->next;
            
                if (currStop->prev != NULL) {
                    currStop->prev->next = currStop->next;
                }
                
                if (nextStop != NULL) {
                    nextStop->prev = currStop->prev;
                }
                free(currStop->name);
                freeRoutesAtStopArray(currStop->routesAtStop, 
                                            currStop->n_routes);
                free(currStop);
                free(s_name);
                return headStop;
            } 
        }

        else{
            currStop = currStop->next;
        }   
    }
    free(s_name);
    return headStop;   
}  
    

void removesStopFromAllRoutes(char* s_name, route* currRoute, route *nextRoute, 
RoutePathNode* currNode, RoutePathNode* nextNode, ListOfRoutes* r_list, 
                                    ListOfStops* s_list){

    /* iterates through routes and removes all nodes that point to the stop */
    currRoute = r_list->head;
    while(currRoute != NULL){

        nextRoute = currRoute->next;

        /* checks if stop is in route*/
        if(stopIsInRoute(r_list, currRoute->name, s_name)){
            
            /* iterates through routePathNodes*/
            currNode = currRoute->path->orig;
            
            while(currNode!=NULL){
                
                nextNode = currNode->next;

                /* if currNode name matches with stop name */
                if(strcmp(currNode->Stop->name, s_name) == 0){

                    /* node is route path orig  */
                    if(stopIsRouteOrigin(r_list, currRoute->name, 
                                                currNode->Stop->name)){
                        
                        /* removes node and updates total cost and duration*/
                        currRoute->total_cost -= currNode->cost;
                        currRoute->total_duration -= currNode->duration;
                        currRoute->path->orig = currNode->next  ;
                        if (currNode->next != NULL){
                            currNode->next->prev = NULL;
                        }
                        --currRoute->n_stops;
                        free(currNode);
                        currNode = currRoute->path->orig;
                    }   
                    
                    /* node is route path dest */
                    else if(stopIsRouteDestination(r_list, currRoute->name, 
                                            currNode->Stop->name)){
                        
                        /* removes node and updates total cost and duration*/
                        currRoute->total_cost -= currNode->prev->cost;
                        currRoute->total_duration -= currNode->prev->duration;
                    
                        if (currNode->prev != NULL) {
                            currNode->prev->cost = 0;
                            currNode->prev->duration = 0;
                            currNode->prev->next = NULL;
                        }
                        currRoute->path->dest = currNode->prev;
                        --currRoute->n_stops;
                        free(currNode);
                        currNode = currRoute->path->dest;
                    }
                    
                    /* stop is in the middle of the route */
                    else{
                       /*removes node,total cost and duration remain the same*/
                        currNode->prev->next = currNode->next;
                        if(currNode->next != NULL){
                            currNode->next->prev = currNode->prev;
                        }
                        --currRoute->n_stops;
                        free(currNode);
                        currNode = nextNode;
                    }

                    /* if after removing a node n_stops < 2, removes route */
                    if(currRoute->n_stops < 2){
                        removesRoute(currRoute->name, r_list, s_list);
                        break;
                    }
                }
            
                else{
                currNode = nextNode;
                }   
            }
        }
        currRoute = nextRoute;
    }
}
