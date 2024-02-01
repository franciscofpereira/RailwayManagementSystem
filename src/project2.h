/* 
    * iaed-23 - ist1107502 - project1 
    * Francisco Ferro Pereira - LEIC-T
    * File: project2.h
    * Description: Definition of macros, structs and prototypes.
*/

#ifndef PROJECT2_H
#define PROJECT2_H

/*-----------------------------------------------------------------------------
                                    MACROS
-----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
                                    STRUCTS
-----------------------------------------------------------------------------*/


/* Forward declaration */
typedef struct stop_struct stop;
typedef struct route_struct route;
typedef struct RoutePathNode_struct RoutePathNode;


/* STOP STRUCT */
struct stop_struct{
    char* name;
    double lat; 
    double lon;
    int n_routes;
    stop *next;
    stop *prev;
    char** routesAtStop;
};

/* LIST OF STOPS */
typedef struct{
    stop* head;
    stop* tail;
}ListOfStops;


/* ROUTE PATH NODE */
struct RoutePathNode_struct{
    stop* Stop;
    RoutePathNode* next;
    RoutePathNode* prev;
    double cost;
    double duration;
};

/* ROUTE PATH */
typedef struct{
    RoutePathNode* orig;
    RoutePathNode* dest;
}RoutePath;


/* ROUTE STRUCT */
struct route_struct{
    char* name;   
    RoutePath* path;
    int n_stops;
    double total_cost;
    double total_duration;
    route* next;
    route* prev;
};

/* LIST OF ROUTES */
typedef struct{
    route* head;
    route* tail;
}ListOfRoutes;


/*-----------------------------------------------------------------------------
                                    PROTOTYPES
-----------------------------------------------------------------------------*/

/* COMMAND FUNCTIONS */
void command_c(char input[], ListOfRoutes* r_list, ListOfStops* s_list);
void command_p(char input[], ListOfRoutes* r_list, ListOfStops* s_list);
void command_l(char input[], ListOfRoutes* r_list, ListOfStops* s_list);
void command_i(ListOfRoutes* r_list, ListOfStops* s_list);
void command_r(char input[], ListOfRoutes* r_list, ListOfStops* s_list);
void command_e(char input[], ListOfRoutes* r_list, ListOfStops* s_list);
void command_a(ListOfRoutes* r_list, ListOfStops* s_list);


/* STOP FUNCTIONS */
stop* getStop(ListOfStops* list, char* s_name);
stop* createStop(ListOfStops* list, ListOfRoutes* r_list, char* s_name, 
                double s_lat, double s_lon);
void listStops(char input[], ListOfStops* s_list, ListOfRoutes* r_list);
int stopExists(ListOfStops* list, char* s_name);
int stopIsRouteOrigin(ListOfRoutes* l_routes, char* r_name, char* s_name);
int stopIsRouteDestination(ListOfRoutes* r_list,char* r_name,char* s_name);
int stopIsInRoute(ListOfRoutes* l_routes, char* r_name, char* s_name);


/* ROUTE FUNCTIONS */
route* getRoute(ListOfRoutes* list, char* r_name);
route* createRoute(ListOfRoutes* list, ListOfStops* stop, char* r_name);
int routeExists(ListOfRoutes* list, char* r_name);
void listRoutes(char input[], ListOfRoutes* r_list, ListOfStops* s_list);
void listRoutesNoArguments(ListOfRoutes* r_list);
void listRoutesOneArgument(char* r_name, ListOfRoutes* r_list, 
                        ListOfStops* s_list);
void listRoutesTwoArguments(char* r_name, char inv_command[], 
                            ListOfRoutes* list_routes);
int inverseIsValid(char inv_command[]);
int routeIsEmpty(ListOfRoutes* l_routes, char* r_name);



/* LINK FUNCTIONS */
void AddLinks(char input[], ListOfRoutes* r_list, ListOfStops* s_list);
void addStopToRouteOrig(char* r_name, char* s_name, double cost, 
                double duration, ListOfRoutes *r_list, ListOfStops *s_list);

void addStopToRouteDest(char* r_name, char* s_name, double cost, 
                double duration, ListOfRoutes *r_list, ListOfStops *s_list);

void addsStopsToEmptyRoute(char* r_name, char link_orig[], char link_dest[], 
                          double cost, double duration, ListOfRoutes *r_list,
                          ListOfStops *s_list);

int validateLinks(char* r_name, char link_orig[], char link_dest[], 
                 double cost, double duration, 
                 ListOfRoutes* l_routes,ListOfStops* l_stops);


/* INTERSECTION FUNCTIONS */
void listIntersections(ListOfRoutes* r_list, ListOfStops* s_list);
void insertionSortRouteNames(char** routesAtStop, int size, 
                    ListOfRoutes* r_list, ListOfStops* s_list);
int nameIsInArray(char* r_name, stop* currStop);

/* OTHER FUNCTIONS */
int QuotesFound(const char input[]);
int validateMalloc(void* ptr, ListOfRoutes* r_list, ListOfStops* s_list);


RoutePath* createRoutePath(ListOfRoutes* r_list, ListOfStops* s_list);
ListOfRoutes* InitializeListOfRoutes();
ListOfStops* InitializeListOfStops();
void addRouteToList(ListOfRoutes* list, route* new_route);
void addsNameToRoutesAtStop(char*** routesAtStop, int n_routes, char* r_name,
                            ListOfRoutes*r_list, ListOfStops* s_list);
int findBiggestRouteName(char** routesAtStop, int size);
void addStopToList(ListOfStops* list, stop* new_stop);




/* MEMORY MANAGEMENT FUNCTIONS */

/* routes */
void freesRoutePathNodes(RoutePath* path);
void freesRoutePath(RoutePath* path);
void freesAllRoutes(ListOfRoutes* r_list);

/* stops */
void freeRoutesAtStopArray(char** routesAtStop, int n_routes);
void freesAllStops(ListOfStops* s_list);

void freesAllMemory(ListOfRoutes* r_list, ListOfStops* s_list);

void removesRoute(char input[], ListOfRoutes* r_list, ListOfStops* s_list);
void removeRouteNameFromArray(char* r_name, stop* s);
stop* removesStop(char input[], ListOfRoutes* r_list, ListOfStops* s_list);
void removesStopFromAllRoutes(char* s_name, route* currRoute, route *nextRoute, 
RoutePathNode* currNode, RoutePathNode* nextNode, ListOfRoutes* r_list, 
                                    ListOfStops* s_list);

#endif
