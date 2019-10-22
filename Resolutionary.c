//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+\\
//=..._____.._____.._____.._____.._......_..._.._____.._.._____.._..._.._____.._____.._..._...=\\
//+..|  _  ||  ___||  ___||  _  || |....| |.| ||_   _|| ||  _  || \.| ||  _  ||  _  || |.| |..+\\
//=..| |_| || |___.| |___.| |.| || |....| |.| |..| |..| || |.| ||  \| || |_| || |_| || |_| |..=\\
//+..|    _||  ___||___  || |.| || |....| |.| |..| |..| || |.| || \   ||  _  ||    _|.\   /...+\\
//=..| |\ \.| |___..___| || |_| || |___.| |_| |..| |..| || |_| || |\  || | | || |\ \...| |....=\\
//+..|_|.\_\|_____||_____||_____||_____||_____|..|_|..|_||_____||_|.\_||_|.|_||_|.\_\..|_|....+\\
//=                                                                                           =\\
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+\\
//                               THE GAME SCALING SYSTEM / Version 1.0 / by lcl 2017 - 2018    \\

// Settings for drawing the debug overlay
#define RES_DEBUG 1                     // Enable (1) or disable (0) debugging
#define RES_DEBUG_UPDATE_FREQ 5         // Update debug overlay every # frames
#define RES_DEBUG_NAMES_PER_LINE 5      // The count of actor names to print before a line change
#define RES_DEBUG_PRINT_MAX_LENGTH 2048 // Maximum length of the debug overlay text
#define RES_DEBUG_FONT_CHAR_COUNT 94    // Amount of letters in the debug font array
#define RES_DEBUG_FONT_DOT_COUNT 26     // 5x5 chars represent a single letter (+1 for NULL)
#define RES_DEBUG_FONT_DOT_SIZE 1       // Bigger value -> bigger pixels -> bigger text
#define RES_DEBUG_FONT_WIDTH 5          // Width of a letter (in dots)
#define RES_DEBUG_FONT_SPACING 6        // Distance between x coordinates of two letters (in dots)
#define RES_DEBUG_FONT_LINE_SPACING 7   // Distance between y coordinates of two lines (in dots)

// Default settings for the overlay, display everything
#define RES_DEBUG_SETTINGS \
(RES_DEBUG_DRAW_OVERLAY | RES_DEBUG_LIST_DYNAMIC | RES_DEBUG_LIST_STATIC | RES_DEBUG_LIST_CLICKED)

// Flags for controlling the overlay
enum resDebugSettingsEnum
{
    RES_DEBUG_DRAW_OVERLAY = (1 << 0),
    RES_DEBUG_LIST_DYNAMIC = (1 << 1),
    RES_DEBUG_LIST_STATIC  = (1 << 2),
    RES_DEBUG_LIST_CLICKED = (1 << 3)
};

#define RES_VERSION_NUMBER 1.0   // Version number

// The actor type settings that control how Resolutionary handles an actor.
// Dynamic is the default setting for all actors.
#define RES_ACTOR_DYNAMIC 0      // Dynamic actors will be redrawn every frame
#define RES_ACTOR_STATIC 1       // Static actors will be redrawn only when requested
#define RES_ACTOR_NOT_DRAWABLE 2 // This option is for actors that don't need to be drawn

#define RES_CLONENAME_SIZE 42    // Characters needed for containing any possible clonename

#define RES_FALSE 0              // False for making some bits of the code clearer
#define RES_TRUE 1               // True for making some bits of the code clearer

#define RES_WIDTH_LIMITED 0      // New resolution's aspect ratio is smaller than original
#define RES_HEIGHT_LIMITED 1     // New resolution's aspect ratio is greater than original

#define RES_MOUSE_UP 0           // Mouse state 0
#define RES_MOUSE_DOWN 1         // Mouse state 1

// Used for accessing mouse data arrays
enum resMouseButtonsEnum
{
    RES_MOUSE_LEFT = 0,
    RES_MOUSE_RIGHT,
    RES_MOUSE_MIDDLE,
    RES_MOUSE_WHEEL_UP,
    RES_MOUSE_WHEEL_DOWN,
    RES_MOUSE_BUTTONS     // Number of supported mouse buttons (5)
};

// Definition of the actor object struct
typedef struct resActorObjectStruct
{
    Actor *actor;                         // Pointer to the actor
    char cName[RES_CLONENAME_SIZE];       // The actor's clonename
    int type;                             // The actor's type (dynamic, static or not drawable)
    int zLayer;                           // The value used for setting the drawing order
    char parentCName[RES_CLONENAME_SIZE]; // The actor's parent's clonename
    struct resActorObjectStruct *next;    // Pointer to the next item in the list
}resActorObject;

// Definition of the Resolutionary system controller struct
struct resControllerStruct
{
    int systemOn;             // Whether the system is currently running (1) or not (0)

    double scale;             // The scale to resize the game to
    int originalScreenWidth;  // The width of the original resolution
    int originalScreenHeight; // The height of the original resolution
    int scaledScreenWidth;    // The width of the resolution to scale to
    int scaledScreenHeight;   // The height of the resolution to scale to
    int updateStaticLayer;    // A flag to be raised when the static layer needs an update
    int backgroundColor[3];   // The rgb of the color to use as background color
    int borderColor[3];       // The rgb of the color to set for the 'black bars'
    int limitingDimension;    // Is the scale value determined by width or height
    int borderSize;           // The size of the black bars
    int mouseXOffset;         // The xscreen coordinate where the scaled view begins
    int mouseYOffset;         // The yscreen coordinate where the scaled view begins

    Actor *mButtonActors[RES_MOUSE_BUTTONS];   // Clicked actors by mouse button
    char mButtonTopActorCName[RES_MOUSE_BUTTONS][RES_CLONENAME_SIZE]; // ...and of those the one
                                                                      // with the highest z depth
    int mButtonActorCount[RES_MOUSE_BUTTONS];  // Amount of clicked actors by mouse button
    int mButtonState[RES_MOUSE_BUTTONS];       // Mouse button states
    enum resMouseButtonsEnum activeButton;     // The last active mouse button

    Actor *pView; // Pointer to view actor, for avoiding fetching it again every frame

    resActorObject *dynamicActorListHead; // The beginning of the list of dynamic actors
    resActorObject *dynamicActorListTail; // The end of the list of dynamic actors
    resActorObject *staticActorListHead;  // The beginning of the list of static actors
    resActorObject *staticActorListTail;  // The end of the list of static actors

    unsigned int debugAllocatedByteCount; // Number of bytes currently allocated for the system
    int debugDynamicCount;                // Number of dynamic actors active
    int debugStaticCount;                 // Number of static actors active
}resController;

int resXmouse; // Use this instead of normal xmouse, this is relative to the scale
int resYmouse; // Use this instead of normal ymouse, this is relative to the scale

// Resolutionary also adds following five actor variables to a project:
// resActorType           - for controlling which actors are drawn and when they are drawn
// resActorZLayer         - for managing the drawing order of actors
// resHasParent           - for knowing whether actor's coordinates are absolute or relative
// resParentName          - for fetching a pointer to an actor's parent when needed
// resIsOnActorObjectList - for knowing which actors are already listed and which are not

// Functions for configuring and using Resolutionary
void resChangeActorType(const char *actorName, int type);
void resChangeActorZLayer(const char *actorName, int zLayer);
void resChangeActorParent(const char *actorName, const char *parentName);
void resSetBackgroundColor(int rValue, int gValue, int bValue);
void resSetBorderColor(int rValue, int gValue, int bValue);
void resStart(int originalWidth, int originalHeight);
void resQuit(void);
int resOnMouseClick(enum resMouseButtonsEnum mButtonNumber);
int resOnMouseRelease(enum resMouseButtonsEnum mButtonNumber);
int resOnMouseButtonDown(enum resMouseButtonsEnum mButtonNumber);
int resOnMouseButtonUp(enum resMouseButtonsEnum mButtonNumber);
int resIsMouseButtonDown(enum resMouseButtonsEnum mButtonNumber);
int resIsMouseButtonUp(enum resMouseButtonsEnum mButtonNumber);
void resRequestStaticLayerUpdate(void);

// Resolutionary's internal functions for managing the actor lists
resActorObject *resCreateActorObjectList(Actor *a);
resActorObject *resAddActorObject(Actor *a);
resActorObject *resSearchActorObject(Actor *a, resActorObject **prev);
int resDeleteActorObject(Actor *a);
int resDeleteActorObjectByPointer(resActorObject *del, resActorObject *prev);
int resUnlinkActorObject(resActorObject *del, resActorObject *prev);
void resDeleteActorObjectLists(void);

// Resolutionary's internal functions for drawing the different layers
void resDrawDynamicLayer(void);
void resDrawStaticLayer(void);
void resSendStaticLayerActivationEvent(void);
void resHandleStaticLayerActivationEvent(void);
void resDisplayDebugOverlay(void);

// Resolutionary's internal functions for miscellaneous tasks
void resUpdateMouse(void);
int resUpdateMouseButtonDown(enum resMouseButtonsEnum mButtonNumber);
void resUpdateMouseButtonUp(enum resMouseButtonsEnum mButtonNumber);
int resSafeStrcat(char *destination, const char *source, int maxLen);
double resLimit(double value, double minVal, double maxVal);
void resDisableMouseEvents(const char *actorName);
void resEnableMouseEvents(const char *actorName);
int resCheckActorExistence(const char *actorName);
int resActorIsListed(Actor *a);



// -------------------- Functions for configuring and using Resolutionary -------------------- //

// This function sets an actor's type to dynamic, static or not drawable
// actorName - name of the actor
// type - the type to be set for the actor, has to be one of the following:
//     - RES_ACTOR_DYNAMIC (default)
//     - RES_ACTOR_STATIC
//     - RES_ACTOR_NOT_DRAWABLE
void resChangeActorType(const char *actorName, int type)
{
    Actor *a = NULL;                // Pointer for the actor
    char cName[RES_CLONENAME_SIZE]; // Clonename for searching the actor

    // Support for Event Actor, Parent Actor, Creator Actor and Collide Actor
    if (strcmp(actorName, "Event Actor") == 0) strcpy(cName, clonename);
    else if (strcmp(actorName, "Parent Actor") == 0) strcpy(cName, parent.clonename);
    else if (strcmp(actorName, "Creator Actor") == 0) strcpy(cName, creator.clonename);
    else if (strcmp(actorName, "Collide Actor") == 0) strcpy(cName, collide.clonename);
    else strcpy(cName, actorName);

    // Abort if the actor doesn't exist
    if (!resCheckActorExistence(cName)) return;

    a = getclone(cName);    // Get the actor pointer according to cName

    if (a->resIsOnActorObjectList) // If the actor is currently listed
    {
        resDeleteActorObject(a); // Remove the actor from its current list
        a->resActorType = type;  // Change the actor's type
        resAddActorObject(a);    // Add the actor back to the correct list
    }
    else
        a->resActorType = type; // Set the actor's type
}

// This function sets an actor's z layer. Actors are drawn in order by their z layer,
// from lowest to highest. So, actors with a higher z layer will be drawn on top of the
// actors with a lower z layer. The number of an actor's z layer can be positive or negative
// actorName - name of the actor
// zLayer - the number of the z layer to assign to the actor
void resChangeActorZLayer(const char *actorName, int zLayer)
{
    Actor *a = NULL;                // Pointer for the actor
    char cName[RES_CLONENAME_SIZE]; // Clonename for searching the actor

    // Support for Event Actor, Parent Actor, Creator Actor and Collide Actor
    if (strcmp(actorName, "Event Actor") == 0) strcpy(cName, clonename);
    else if (strcmp(actorName, "Parent Actor") == 0) strcpy(cName, parent.clonename);
    else if (strcmp(actorName, "Creator Actor") == 0) strcpy(cName, creator.clonename);
    else if (strcmp(actorName, "Collide Actor") == 0) strcpy(cName, collide.clonename);
    else strcpy(cName, actorName);

    // Abort if the actor doesn't exist
    if (!resCheckActorExistence(cName)) return;

    a = getclone(cName);        // Get the actor pointer according to cName

    if (a->resIsOnActorObjectList) // If the actor is currently listed
    {
        resDeleteActorObject(a);    // Remove the actor from its current position
        a->resActorZLayer = zLayer; // Set the actor's z layer
        resAddActorObject(a);       // Add the actor back to the list to the correct position
    }
    a->resActorZLayer = zLayer; // Set the actor's z layer
}

// This function lets Resolutionary know that an actor has a parent
//
// *** Important ***
// Make sure to always use this function together with Game Editor's ChangeParent function if
// the parented actor is to be drawn by Resolutionary. If the system isn't notified of the change
// in the actor's parenting, the actor will be drawn in a wrong position
//
// actorName - name of the child actor
// parentName - name of the parent actor, if removing a parent, set this to "(none)"
void resChangeActorParent(const char *actorName, const char *parentName)
{
    Actor *a = NULL, *a2 = NULL; // Pointers for the child actor and the parent actor
    char cName[RES_CLONENAME_SIZE],  // Clonename for searching the actor
         cName2[RES_CLONENAME_SIZE]; // Clonename for searching the parent actor

    // Support for Event Actor, Parent Actor, Creator Actor and Collide Actor
    if (strcmp(actorName, "Event Actor") == 0) strcpy(cName, clonename);
    else if (strcmp(actorName, "Parent Actor") == 0) strcpy(cName, parent.clonename);
    else if (strcmp(actorName, "Creator Actor") == 0) strcpy(cName, creator.clonename);
    else if (strcmp(actorName, "Collide Actor") == 0) strcpy(cName, collide.clonename);
    else strcpy(cName, actorName);

    // Abort if the child actor doesn't exist
    if (!resCheckActorExistence(cName)) return;

    a = getclone(cName); // Get the actor pointer according to cName

    // If parentName is set to (none)
    if (strcmp(parentName, "(none)") == 0)
    {
        a->resHasParent = RES_FALSE;        // Set child actor's hasParent to false
        strcpy(a->resParentName, "(none)"); // Set child actor's parentName
        return;                             // Finish
    }

    // Support for Event Actor, Parent Actor, Creator Actor and Collide Actor
    if (strcmp(parentName, "Event Actor") == 0) strcpy(cName2, clonename);
    else if (strcmp(parentName, "Parent Actor") == 0) strcpy(cName2, parent.clonename);
    else if (strcmp(parentName, "Creator Actor") == 0) strcpy(cName2, creator.clonename);
    else if (strcmp(parentName, "Collide Actor") == 0) strcpy(cName2, collide.clonename);
    else strcpy(cName2, parentName);

    // Abort if the parent actor doesn't exist
    if (!resCheckActorExistence(cName2)) return;

    a2 = getclone(cName2);                   // Get the parent actor pointer according to cName2
    a->resHasParent = RES_TRUE;              // Set child actor's hasParent to true
    strcpy(a->resParentName, a2->clonename); // Set child actor's parentName

    if (a->resIsOnActorObjectList) // If the actor is currently listed
    {
        resActorObject *ptr = resSearchActorObject(a, NULL); // Find the corresponding actor object

        if (ptr) strcpy(ptr->parentCName, a2->clonename); // Update the stored parent clonename
    }
}

// This function sets the background color of the game, it might be desirable to set it to be
// the same as the project's background color in Config -> Game properties -> Game background color
// rValue - the red color component, input a value between 0 - 255
// gValue - the green color component, input a value between 0 - 255
// bValue - the blue color component, input a value between 0 - 255
void resSetBackgroundColor(int rValue, int gValue, int bValue)
{
    Actor *pBackground = NULL; // Pointer to background actor

    resController.backgroundColor[0] = rValue;
    resController.backgroundColor[1] = gValue;
    resController.backgroundColor[2] = bValue;

    pBackground = getclone("resBackground"); // Get pointer to background actor

    if (pBackground->cloneindex != -1) // If background actor exists, color it as well
    {
        pBackground->r = resController.backgroundColor[0];
        pBackground->g = resController.backgroundColor[1];
        pBackground->b = resController.backgroundColor[2];
    }
}

// This function sets the color of the borders that will be shown on the edges of the screen if
// the new resolution's aspect ratio is different from the original one's. Typically these borders
// are black and hence are sometimes referred to as "black bars"
//
// *** Important ***
// This function has to be called before calling resStart() (i.e. before the borders are created)
// for the specified color to be applied on the borders
//
// rValue - the red color component, input a value between 0 - 255
// gValue - the green color component, input a value between 0 - 255
// bValue - the blue color component, input a value between 0 - 255
void resSetBorderColor(int rValue, int gValue, int bValue)
{
    resController.borderColor[0] = rValue;
    resController.borderColor[1] = gValue;
    resController.borderColor[2] = bValue;
}

// This function creates the Resolutionary system actors and initializes and starts the system
// originalWidth - the width of the original resolution
// originalHeight - the height of the original resolution
void resStart(int originalWidth, int originalHeight)
{
    double widthRatio;  // A variable for the ratio of the change in the resolution's width
    double heightRatio; // A variable for the ratio of the change in the resolution's height

    // Pointers for all the Resolutionary system actors that will be created
    Actor *pBarOne = NULL,
          *pBarTwo = NULL,
          *pBackground = NULL,
          *pActorDetector = NULL,
          *pMouse = NULL,
          *pClickDetector = NULL,
          *pDynamicLayer = NULL,
          *pStaticLayer = NULL;

    resController.pView = getclone("view"); // Get the pointer to view actor
    // Initialize all mouse buttons' states
    resController.mButtonState[RES_MOUSE_LEFT] =
    resController.mButtonState[RES_MOUSE_RIGHT] =
    resController.mButtonState[RES_MOUSE_MIDDLE] =
    resController.mButtonState[RES_MOUSE_WHEEL_UP] =
    resController.mButtonState[RES_MOUSE_WHEEL_DOWN] = RES_MOUSE_UP;

    // Create the Resolutionary system actors
    pBackground = CreateActor("resBackground", "resBackground", "(none)", "(none)", 0, 0, true);
    pActorDetector = CreateActor("resActorDetector", "icon", "(none)", "(none)", 0, 0, true);
    pMouse = CreateActor("resMouse", "resMouse", "(none)", "(none)", 0, 0, true);
    pClickDetector = CreateActor("resMouseClickDetector", "icon", "(none)", "(none)", 0, 0, true);
    pDynamicLayer = CreateActor("resDynamicLayerCanvas", "icon", "(none)", "(none)", 0, 0, true);
    pStaticLayer = CreateActor("resStaticLayerCanvas", "icon", "(none)", "(none)", 0, 0, true);

    // Initialize Background
    ChangeParent(pBackground->clonename, "view");       // Parent to view
    ChangeZDepth(pBackground->clonename, 1.1);          // Set z depth
    CollisionState(pBackground->clonename, DISABLE);    // Disable collision
    EventDisable(pBackground->clonename, EVENTALL);     // Disable all events
    pBackground->x = resController.pView->width * 0.5;  // Set x coordinate in relation to view
    pBackground->y = resController.pView->height * 0.5; // Set y coordinate in relation to view
    pBackground->r = resController.backgroundColor[0];  // Set color r component
    pBackground->g = resController.backgroundColor[1];  // Set color g component
    pBackground->b = resController.backgroundColor[2];  // Set color b component


    // Initialize ActorDetector
    ChangeParent(pActorDetector->clonename, "view");  // Parent to view
    ChangeZDepth(pActorDetector->clonename, 0.0);     // Set z depth
    resDisableMouseEvents(pActorDetector->clonename); // Disable mouse events
    pActorDetector->x = 0;                            // Set x coordinate in relation to view
    pActorDetector->y = 0;                            // Set y coordinate in relation to view

    // Initialize Mouse
    resChangeActorType(pMouse->clonename, RES_ACTOR_NOT_DRAWABLE); // Set type to not drawable
    resDisableMouseEvents(pMouse->clonename);                      // Disable mouse events
    pMouse->xscreen = 0;                                           // Set initial xscreen position
    pMouse->yscreen = 0;                                           // Set initial yscreen position

    // Initialize ClickDetector
    ChangeParent(pClickDetector->clonename, "view");    // Parent to view
    ChangeZDepth(pClickDetector->clonename, 1.4);       // Set z depth
    CollisionState(pClickDetector->clonename, DISABLE); // Disable collision
    pClickDetector->x = 0;                              // Set x coordinate in relation to view
    pClickDetector->y = 0;                              // Set y coordinate in relation to view

    // Initialize DynamicLayerCanvas
    ChangeParent(pDynamicLayer->clonename, "view");    // Parent to view
    ChangeZDepth(pDynamicLayer->clonename, 1.3);       // Set z depth
    CollisionState(pDynamicLayer->clonename, DISABLE); // Disable collision
    resDisableMouseEvents(pDynamicLayer->clonename);   // Disable mouse events
    pDynamicLayer->x = 0;                              // Set x coordinate in relation to view
    pDynamicLayer->y = 0;                              // Set y coordinate in relation to view

    // Initialize StaticLayerCanvas
    ChangeParent(pStaticLayer->clonename, "view");    // Parent to view
    ChangeZDepth(pStaticLayer->clonename, 1.2);       // Set z depth
    CollisionState(pStaticLayer->clonename, DISABLE); // Disable collision
    resDisableMouseEvents(pStaticLayer->clonename);   // Disable mouse events
    pStaticLayer->x = 0;                              // Set x coordinate in relation to view
    pStaticLayer->y = 0;                              // Set y coordinate in relation to view

    resController.originalScreenWidth = originalWidth;   // Set original screen width
    resController.originalScreenHeight = originalHeight; // Set original screen height

    widthRatio = resController.pView->width / (double)originalWidth;    // Calculate width ratio
    heightRatio = resController.pView->height / (double)originalHeight; // Calculate height ratio

    // Choose the smaller ratio as the scale
    resController.scale = min(widthRatio, heightRatio);

    // Calculate dimensions of the scaled screen
    resController.scaledScreenWidth = originalWidth * resController.scale;
    resController.scaledScreenHeight = originalHeight * resController.scale;

    if (abs(widthRatio - heightRatio) > 0.001) // If the game's aspect ratio has changed
    {
        // Set the limiting dimension according to the smaller of the resolution change ratios
        resController.limitingDimension =
            (widthRatio < heightRatio) ? RES_WIDTH_LIMITED : RES_HEIGHT_LIMITED;

        // If the size of the screen is limited by width, black bars have to
        // be added to the top and bottom of the screen
        if (resController.limitingDimension == RES_WIDTH_LIMITED)
        {
            // Calculate the excess space to be covered by the black bars
            int excessSpaceHeight = resController.pView->height - resController.scaledScreenHeight;

            resController.borderSize = excessSpaceHeight * 0.5;    // Set the size of a black bar
            resController.mouseXOffset = 0;                        // Set the mouse x offset
            resController.mouseYOffset = resController.borderSize; // Set the mouse y offset

            // Move the canvases just below where the bottom edge of the upper black bar will be
            pDynamicLayer->y = pStaticLayer->y = excessSpaceHeight * 0.5;

            // Create the upper black bar and set its position
            pBarOne = CreateActor("resBorders", "resBarHorizontal", "view", "(none)", 0, 0, true);
            pBarOne->x = pBarOne->width * 0.5;
            pBarOne->y = - (pBarOne->height * 0.5 - excessSpaceHeight * 0.5);

            // Create the lower black bar and set its position
            pBarTwo = CreateActor("resBorders", "resBarHorizontal", "view", "(none)", 0, 0, true);
            pBarTwo->x = pBarTwo->width * 0.5;
            pBarTwo->y = resController.pView->height +
                             (pBarTwo->height * 0.5 - excessSpaceHeight * 0.5);
        }
        // If the size of the screen is limited by height, black bars have to
        // be added to the left and right edges of the screen
        else if (resController.limitingDimension == RES_HEIGHT_LIMITED)
        {
            // Calculate the excess space to be covered by the black bars
            int excessSpaceWidth = resController.pView->width - resController.scaledScreenWidth;

            resController.borderSize = excessSpaceWidth * 0.5;     // Set the size of a black bar
            resController.mouseXOffset = resController.borderSize; // Set the mouse x offset
            resController.mouseYOffset = 0;                        // Set the mouse y offset

            // Move the canvases just to the right of where the right edge of
            // the left side black bar will be
            pDynamicLayer->x = pStaticLayer->x = excessSpaceWidth * 0.5;

            // Create the left side black bar and set its position
            pBarOne = CreateActor("resBorders", "resBarVertical", "view", "(none)", 0, 0, true);
            pBarOne->x = - (pBarOne->width * 0.5 - excessSpaceWidth * 0.5);
            pBarOne->y = pBarOne->height * 0.5;

            // Create the right side black bar and set its position
            pBarTwo = CreateActor("resBorders", "resBarVertical", "view", "(none)", 0, 0, true);
            pBarTwo->x = resController.pView->width +
                             (pBarTwo->width * 0.5 - excessSpaceWidth * 0.5);
            pBarTwo->y = pBarTwo->height * 0.5;
        }

        // Color the black bars (despite the name they aren't forced to be black)
        pBarOne->r = pBarTwo->r = resController.borderColor[0];
        pBarOne->g = pBarTwo->g = resController.borderColor[1];
        pBarOne->b = pBarTwo->b = resController.borderColor[2];

        // Set the z depth for the black bars
        ChangeZDepth(pBarOne->clonename, 2.0);
        ChangeZDepth(pBarTwo->clonename, 2.0);

        // Disable collision for the black bars
        CollisionState(pBarOne->clonename, DISABLE);
        CollisionState(pBarTwo->clonename, DISABLE);

        // Disable mouse events for the black bars
        resDisableMouseEvents(pBarOne->clonename);
        resDisableMouseEvents(pBarTwo->clonename);
    }

    // Update static layer to make sure that the background gets colored at start
    resSendStaticLayerActivationEvent();

    resController.systemOn = RES_TRUE; // Set system state variable to true
}

// This function quits the Resolutionary system's execution, destroys all Resolutionary
// system actors and frees all allocated memory
void resQuit(void)
{
    DestroyActor("resBackground");
    DestroyActor("resActorDetector");
    DestroyActor("resMouse");
    DestroyActor("resMouseClickDetector");
    DestroyActor("resDynamicLayerCanvas");
    DestroyActor("resStaticLayerCanvas");
    DestroyActor("resBorders");
    resDeleteActorObjectLists();

    resController.systemOn = RES_FALSE; // Set system state variable to false
}

// This function checks if a specified mouse button was just pressed down
// on the actor calling this function. This function is meant to be used
// as a way to replicate Game Editor's default mouse click behavior where
// only the actor with the highest z depth at the cursor's position reacts
// to mouse input
//
// *** Important ***
// Due to the way Resolutionary's mouse input works, this function can
// only be used in an Activation Event from resMouseClickDetector
// -> For more information, see Resolutionary's documentation
// -> For checking if a specified mouse button is being held down,
//    use the function resIsMouseButtonDown
//
// mButtonNumber - the mouse button to check, must be one of the following:
//     - RES_MOUSE_LEFT
//     - RES_MOUSE_RIGHT
//     - RES_MOUSE_MIDDLE
//     - RES_MOUSE_WHEEL_UP
//     - RES_MOUSE_WHEEL_DOWN
// returns: 1 if true, 0 if false
int resOnMouseClick(enum resMouseButtonsEnum mButtonNumber)
{
    return (!strcmp(resController.mButtonTopActorCName[mButtonNumber], clonename) &&
            resController.mButtonState[mButtonNumber] &&
            resController.activeButton == mButtonNumber);
}

// This function checks if a specified mouse button was just released
// on the actor calling this function. This function is meant to be used
// as a way to replicate Game Editor's default mouse click behavior where
// only the actor with the highest z depth at the cursor's position reacts
// to mouse input
//
// *** Important ***
// Due to the way Resolutionary's mouse input works, this function can
// only be used in an Activation Event from resMouseClickDetector
// -> For more information, see Resolutionary's documentation
// -> For checking if a specified mouse button is not being held down,
//    use the function resIsMouseButtonUp
//
// mButtonNumber - the mouse button to check, must be one of the following:
//     - RES_MOUSE_LEFT
//     - RES_MOUSE_RIGHT
//     - RES_MOUSE_MIDDLE
//     - RES_MOUSE_WHEEL_UP
//     - RES_MOUSE_WHEEL_DOWN
// returns: 1 if true, 0 if false
int resOnMouseRelease(enum resMouseButtonsEnum mButtonNumber)
{
    return (!strcmp(resController.mButtonTopActorCName[mButtonNumber], clonename) &&
            !resController.mButtonState[mButtonNumber] &&
            resController.activeButton == mButtonNumber);
}

// This function checks if a specified mouse button was just pressed down
//
// *** Important ***
// Due to the way Resolutionary's mouse input works, this function can
// only be used in an Activation Event from resMouseClickDetector
// -> For more information, see Resolutionary's documentation
// -> For checking if a specified mouse button is being held down,
//    use the function resIsMouseButtonDown
//
// mButtonNumber - the mouse button to check, must be one of the following:
//     - RES_MOUSE_LEFT
//     - RES_MOUSE_RIGHT
//     - RES_MOUSE_MIDDLE
//     - RES_MOUSE_WHEEL_UP
//     - RES_MOUSE_WHEEL_DOWN
// returns: 1 if true, 0 if false
int resOnMouseButtonDown(enum resMouseButtonsEnum mButtonNumber)
{
    return (resController.mButtonState[mButtonNumber] &&
            resController.activeButton == mButtonNumber);
}

// This function checks if a specified mouse button was just released
//
// *** Important ***
// Due to the way Resolutionary's mouse input works, this function can
// only be used in an Activation Event from resMouseClickDetector
// -> For more information, see Resolutionary's documentation
// -> For checking if a specified mouse button is not being held down,
//    use the function resIsMouseButtonUp
//
// mButtonNumber - the mouse button to check, must be one of the following:
//     - RES_MOUSE_LEFT
//     - RES_MOUSE_RIGHT
//     - RES_MOUSE_MIDDLE
//     - RES_MOUSE_WHEEL_UP
//     - RES_MOUSE_WHEEL_DOWN
// returns: 1 if true, 0 if false
int resOnMouseButtonUp(enum resMouseButtonsEnum mButtonNumber)
{
    return (!resController.mButtonState[mButtonNumber] &&
            resController.activeButton == mButtonNumber);
}

// This function can be used to check if a specified mouse button is down
// mButtonNumber - the mouse button to check, must be one of the following:
//     - RES_MOUSE_LEFT
//     - RES_MOUSE_RIGHT
//     - RES_MOUSE_MIDDLE
//     - RES_MOUSE_WHEEL_UP
//     - RES_MOUSE_WHEEL_DOWN
// returns: 1 if true, 0 if false
int resIsMouseButtonDown(enum resMouseButtonsEnum mButtonNumber)
{
    return (resController.mButtonState[mButtonNumber]);
}

// This function can be used to check if a specified mouse button is up
// mButtonNumber - the mouse button to check, must be one of the following:
//     - RES_MOUSE_LEFT
//     - RES_MOUSE_RIGHT
//     - RES_MOUSE_MIDDLE
//     - RES_MOUSE_WHEEL_UP
//     - RES_MOUSE_WHEEL_DOWN
// returns: 1 if true, 0 if false
int resIsMouseButtonUp(enum resMouseButtonsEnum mButtonNumber)
{
    return (!resController.mButtonState[mButtonNumber]);
}

// This function can be used to manually tell Resolutionary to redraw the static layer
void resRequestStaticLayerUpdate(void)
{
    resController.updateStaticLayer = RES_TRUE; // Raise the static layer update flag
}



// ------------- Resolutionary's internal functions for managing the actor lists ------------- //

// This function creates the linked lists for the drawable dynamic and static actors
// a - pointer to the first actor to be added to the list
// returns: a pointer to the first actor object added to the new list, NULL on failure
resActorObject *resCreateActorObjectList(Actor *a)
{
    resActorObject *ptr = NULL; // Pointer for the new actor object

    // Abort if the actor is not drawable
    if (a->resActorType == RES_ACTOR_NOT_DRAWABLE ||
       (a->resActorType != RES_ACTOR_DYNAMIC && a->resActorType != RES_ACTOR_STATIC))
        return NULL;

    ptr = malloc(sizeof *ptr); // Allocate memory for the new list item

    if (ptr == NULL) return NULL; // Abort if memory allocation failed

    #if RES_DEBUG // If the project is in debug mode
        // Update the system's memory usage variable
        resController.debugAllocatedByteCount += sizeof *ptr;
    #endif

    ptr->actor = a;                   // Get the actor's address
    strcpy(ptr->cName, a->clonename); // Get the actor's clonename
    ptr->type = a->resActorType;      // Get the actor's type
    ptr->zLayer = a->resActorZLayer;  // Get the actor's z layer

    if (a->resHasParent)                            // If the actor has a parent
        strcpy(ptr->parentCName, a->resParentName); // Get the parent's name

    ptr->next = NULL; // As the list is currently being created, there's only one item on
                      // the list so no next item to point to

    // Set the actor variable telling that the actor is currently listed
    a->resIsOnActorObjectList = RES_TRUE;

    // List is being created, the added actor is the only object on the list, so that's where
    // the list begins and where it ends, set the list head and tail pointers to current object
    // (dynamic and static actors have their own lists, so they need to be handled separately)
    if (ptr->type == RES_ACTOR_DYNAMIC)
        resController.dynamicActorListHead = resController.dynamicActorListTail = ptr;
    else if (ptr->type == RES_ACTOR_STATIC)
    {
        resController.staticActorListHead = resController.staticActorListTail = ptr;
        resController.updateStaticLayer = RES_TRUE; // Raise the static layer update flag
    }

    return ptr; // Return a pointer to the object that was added
}

// This function adds an actor to the list of drawable actors
// a - pointer to the actor to be added to the list
// returns: a pointer to the actor object added to the list, NULL on failure
resActorObject *resAddActorObject(Actor *a)
{
    resActorObject *ptr = NULL; // Pointer for the new actor object

    // Abort if the actor is not drawable
    if (a->resActorType == RES_ACTOR_NOT_DRAWABLE ||
       (a->resActorType != RES_ACTOR_DYNAMIC && a->resActorType != RES_ACTOR_STATIC))
        return NULL;

    // If the corresponding actor list is not created yet, create it by adding the current actor
    if ((a->resActorType == RES_ACTOR_DYNAMIC && resController.dynamicActorListHead == NULL) ||
        (a->resActorType == RES_ACTOR_STATIC && resController.staticActorListHead == NULL))
        return resCreateActorObjectList(a);

    ptr = malloc(sizeof *ptr); // Allocate memory for the new list item

    if (ptr == NULL) return NULL; // Abort if memory allocation failed

    #if RES_DEBUG // If the project is in debug mode
        // Update the system's memory usage variable
        resController.debugAllocatedByteCount += sizeof *ptr;
    #endif

    ptr->actor = a;                   // Get the actor's address
    strcpy(ptr->cName, a->clonename); // Get the actor's clonename
    ptr->type = a->resActorType;      // Get the actor's type
    ptr->zLayer = a->resActorZLayer;  // Get the actor's z layer

    if (a->resHasParent)                            // If the actor has a parent
        strcpy(ptr->parentCName, a->resParentName); // Get the parent's name

    // Set the actor variable telling that the actor is currently on a list
    a->resIsOnActorObjectList = RES_TRUE;

    // Find the correct place for the actor on the list and add it there, set the list pointers
    // (dynamic and static actors have their own lists, so they need to be handled separately)
    if (ptr->type == RES_ACTOR_DYNAMIC)
    {
        resActorObject *temp = NULL; // The list iterator pointer
        resActorObject *prev = NULL; // Pointer to the previous object in the list

        temp = resController.dynamicActorListHead; // Set list iterator to dynamic actor list head
        prev = NULL;                               // Set prev to NULL

        // Actor's place in the list depends on it's z layer, find the right place
        // by traversing through the list until an object with a higher or equal z layer is found
        // while maintaining a pointer to the previous object
        while (temp != NULL && temp->zLayer < ptr->zLayer)
        {
            prev = temp;       // Get the pointer to the current object
            temp = temp->next; // Move on to the next object
        }

        // If the right place for the actor is a special case (beginning or end of the list)
        // it needs to be handled separately
        if (temp == resController.dynamicActorListHead) // If beginning of the list
        {
            ptr->next = resController.dynamicActorListHead; // Set the new object's next pointer
            resController.dynamicActorListHead = ptr;       // Set the new object as list head
        }
        else if (prev == resController.dynamicActorListTail) // If end of the list
        {
            prev->next = ptr; // Set the previous object to point to the new object
            ptr->next = NULL; // Set the new object's next pointer to NULL
            resController.dynamicActorListTail = ptr; // Set the new object as list tail
        }
        else // The right place for the actor is a normal case
        {
            ptr->next = prev->next; // Set the new object to point to the next object
            prev->next = ptr;       // Set the previous object to point to the new object
        }
    }
    else if (ptr->type == RES_ACTOR_STATIC)
    {
        resActorObject *temp = NULL; // The list iterator pointer
        resActorObject *prev = NULL; // Pointer to the previous object in the list

        temp = resController.staticActorListHead; // Set list iterator to static actor list head
        prev = NULL;                              // Set prev to NULL

        // Actor's place in the list depends on it's z layer, find the right place
        // by traversing through the list until an object with a higher or equal z layer is found
        // while maintaining a pointer to the previous object
        while (temp != NULL && temp->zLayer < ptr->zLayer)
        {
            prev = temp;       // Get the pointer to the current object
            temp = temp->next; // Move on to the next object
        }

        // If the right place for the actor is a special case (beginning or end of the list)
        // it needs to be handled separately
        if (temp == resController.staticActorListHead) // If beginning of the list
        {
            ptr->next = resController.staticActorListHead; // Set the new object's next pointer
            resController.staticActorListHead = ptr;       // Set the new object as list head
        }
        else if (prev == resController.staticActorListTail) // If end of the list
        {
            prev->next = ptr; // Set the previous object to point to the new object
            ptr->next = NULL; // Set the new object's next pointer to NULL
            resController.staticActorListTail = ptr; // Set the new object as list tail
        }
        else // The right place for the actor is a normal case
        {
            ptr->next = prev->next; // Set the new object to point to the next object
            prev->next = ptr;       // Set the previous object to point to the new object
        }

        resController.updateStaticLayer = RES_TRUE; // Raise the static layer update flag
    }

    return ptr; // Return a pointer to the object that was added
}

// This function searches an actor object from the drawable actor list and also gives
// a pointer to the previous actor object in the list
// a - a pointer to the actor to be searched from the list
// prev - a pointer to a pointer to an actor object, the address of the previous actor object
//     in the list will be stored to the pointer this pointer points to
// returns: a pointer to the actor object that corresponds to the given actor, NULL if no match
resActorObject *resSearchActorObject(Actor *a, resActorObject **prev)
{
    resActorObject *ptr = NULL;  // The list iterator pointer
    resActorObject *temp = NULL; // Pointer to the previous object in the list

    // Dynamic and static actors have their own lists, so they need to be handled separately
    if (a->resActorType == RES_ACTOR_DYNAMIC)
        ptr = resController.dynamicActorListHead;
    else if (a->resActorType == RES_ACTOR_STATIC)
        ptr = resController.staticActorListHead;

    // Traverse through the list while maintaining a pointer to the previous object
    while (ptr != NULL)
    {
        if (ptr->actor == a) // If the matching actor object was found
        {
            if (prev) *prev = temp; // Return pointer to the previous item in the list
            return ptr;             // Return pointer to the matching item in the list
        }
        else // Match wasn't found, continue
        {
            temp = ptr;      // Get the pointer to the current object
            ptr = ptr->next; // Move on to the next object
        }
    }

    return NULL; // List traversed through with no match found, return NULL
}

// This function deletes a given actor's corresponding actor object from the list
// and frees the memory allocated for it
// a - pointer to the actor to be deleted from the list
// returns: 1 on success, 0 if no corresponding actor object was found for the actor
int resDeleteActorObject(Actor *a)
{
    resActorObject *del = NULL;  // Pointer to the object to be deleted
    resActorObject *prev = NULL; // Pointer to the preceding object in the list

    // Get the pointer to the object to be deleted and the pointer to the preceding object
    del = resSearchActorObject(a, &prev);

    if (del == NULL) return 0; // Abort if the searched object was not found

    resUnlinkActorObject(del, prev); // Remove the actor object's linking from the list

    #if RES_DEBUG // If the project is in debug mode
        // Update the system's memory usage variable
        resController.debugAllocatedByteCount -= sizeof *del;
    #endif

    free(del);  // Free the memory previously occupied by the object
    del = NULL; // NULL the pointer

    return 1; // Finish
}

// This function deletes an actor object from the list and frees the memory allocated for it
// del - pointer to the actor object to be deleted from the list
// prev - pointer to the preceding actor object in the list (or NULL if there's none)
// returns: 1 on success, 0 on error
int resDeleteActorObjectByPointer(resActorObject *del, resActorObject *prev)
{
    Actor *a = NULL; // Pointer for the actor

    // Abort if a NULL pointer was given
    if (del == NULL) return 0;

    resUnlinkActorObject(del, prev); // Remove the actor object's linking from the list

    #if RES_DEBUG // If the project is in debug mode
        // Update the system's memory usage variable
        resController.debugAllocatedByteCount -= sizeof *del;
    #endif

    free(del);  // Free the memory previously occupied by the object
    del = NULL; // NULL the pointer

    return 1; // Finish
}

// This function removes an actor object's linking from the list
// del - pointer to the actor object to be unlinked
// prev - pointer to the preceding actor object in the list (or NULL if there's none)
// returns: 1 on success, 0 on error
int resUnlinkActorObject(resActorObject *del, resActorObject *prev)
{
    Actor *a = NULL; // Pointer for the actor

    // Abort if a NULL pointer was given
    if (del == NULL) return 0;

    if (resCheckActorExistence(del->cName)) // Check if the actor object's actor exists
    {
        a = getclone(del->cName);              // Get the actor's address
        // Set the actor variable telling that the actor is not currently on a list
        a->resIsOnActorObjectList = RES_FALSE;
    }

    // Remove the object's linking from the list by making the previous object's link point
    // to the object after the one that will be deleted. Also, handle the special cases if
    // the object to delete is either the head or the tail of the list
    // (dynamic and static actors have their own lists, so they need to be handled separately)
    if (del->type == RES_ACTOR_DYNAMIC)
    {
        if (prev) prev->next = del->next; // If previous object exists, update its link

        // If the object to delete is list head
        if (del == resController.dynamicActorListHead)
        {
            // If next object exists, make it the new list head
            if (del->next) resController.dynamicActorListHead = del->next;
            // Else, the list is empty, set list head and tail to NULL
            else resController.dynamicActorListHead = resController.dynamicActorListTail = NULL;

        }
        // If the object to delete is list tail
        else if (del == resController.dynamicActorListTail)
        {
            // If previous object exists, make it the new list tail
            if (prev) resController.dynamicActorListTail = prev;
            // Else, the list is empty, set list head and tail to NULL
            else resController.dynamicActorListHead = resController.dynamicActorListTail = NULL;
        }
    }
    else if (del->type == RES_ACTOR_STATIC)
    {
        if (prev) prev->next = del->next; // If previous object exists, update its link

        // If the object to delete is list head
        if (del == resController.staticActorListHead)
        {
            // If next object exists, make it the new list head
            if (del->next) resController.staticActorListHead = del->next;
            // Else, the list is empty, set list head and tail to NULL
            else resController.staticActorListHead = resController.staticActorListTail = NULL;
        }
        // If the object to delete is list tail
        else if (del == resController.staticActorListTail)
        {
            // If previous object exists, make it the new list tail
            if (prev) resController.staticActorListTail = prev;
            // Else, the list is empty, set list head and tail to NULL
            else resController.staticActorListHead = resController.staticActorListTail = NULL;
        }

        resController.updateStaticLayer = RES_TRUE; // Raise the static layer update flag
    }

    return 1; // Finish
}

// This function deletes all actor objects in the lists and frees the memory allocated for them
void resDeleteActorObjectLists(void)
{
    resActorObject *ptr = NULL;  // The list iterator pointer
    resActorObject *temp = NULL; // Temporary helper pointer

    ptr = resController.dynamicActorListHead; // Set list iterator to dynamic actor list head
    temp = NULL;                              // Set temp to NULL

    while (ptr != NULL) // Traverse through the dynamic actor list
    {
        temp = ptr->next;                               // Get the pointer to the next item
        if (resCheckActorExistence(ptr->cName))             // If the actor exists,
            ptr->actor->resIsOnActorObjectList = RES_FALSE; // make it free for listing again

        #if RES_DEBUG // If the project is in debug mode
            // Update the system's memory usage variable
            resController.debugAllocatedByteCount -= sizeof *ptr;
        #endif

        free(ptr);                                      // Free the object's memory
        ptr = temp;                                     // Move on to the next object
    }

    ptr = resController.staticActorListHead; // Set list iterator to static actor list head
    temp = NULL;                             // Reset temp to NULL

    while (ptr != NULL) // Traverse through the static actor list
    {
        temp = ptr->next;                               // Get the pointer to the next item
        if (resCheckActorExistence(ptr->cName))             // If the actor exists,
            ptr->actor->resIsOnActorObjectList = RES_FALSE; // make it free for listing again

        #if RES_DEBUG // If the project is in debug mode
            // Update the system's memory usage variable
            resController.debugAllocatedByteCount -= sizeof *ptr;
        #endif

        free(ptr);                                      // Free the object's memory
        ptr = temp;                                     // Move on to the next object
    }

    #if RES_DEBUG // If the project is in debug mode
        // Set the actor list count variables to 0
        resController.debugDynamicCount = 0;
        resController.debugStaticCount = 0;
    #endif

    // Set dynamic actor list head and tail to NULL
    resController.dynamicActorListHead = resController.dynamicActorListTail = NULL;
    // Set static actor list head and tail to NULL
    resController.staticActorListHead = resController.staticActorListTail = NULL;
}



// ----------- Resolutionary's internal functions for drawing the different layers ----------- //

// This function draws the dynamic layer
void resDrawDynamicLayer(void)
{
    Actor *a = NULL; // Pointer for the actor to draw
    Actor *p = NULL; // Pointer for an actor's parent
    resActorObject *prev = NULL; // Pointer to the previous item in the list
    resActorObject *ptr = resController.dynamicActorListHead; // List iterator pointer

    int viewX = resController.pView->x; // Get view's x coordinate
    int viewY = resController.pView->y; // Get view's y coordinate
    double scale = resController.scale; // Get the scale for drawing the actor

    erase(0, 0, 0, 1); // Clear the canvas

    // If the static layer update flag is raised, update the static layer
    if (resController.updateStaticLayer) resSendStaticLayerActivationEvent();

    while (ptr != NULL) // Traverse through the list
    {
        if (!resCheckActorExistence(ptr->cName)) // If the actor doesn't exist
        {
            // Get next object's address before the current object is deleted
            resActorObject *temp = ptr->next;

            resDeleteActorObjectByPointer(ptr, prev); // Delete current actor object
            ptr = temp;                       // Move on to the next object
            continue; // There's no actor to draw, so skip the rest of this cycle
        }

        a = ptr->actor; // Get the actor pointer

        if (!a->resHasParent) // If the actor doesn't have a parent
            // Draw the actor
            draw_from(ptr->cName, (a->x - viewX) * scale,
                                  (a->y - viewY) * scale, scale);
        else // If the actor has a parent
        {
            // If the object's parent's clonename is already stored,
            // use that to acquire a pointer to the parent
            if (strlen(ptr->parentCName) > 0) p = getclone(ptr->parentCName);
            else
            {
                p = getclone(a->resParentName);             // Get the pointer to the parent
                strcpy(ptr->parentCName, a->resParentName); // Store the parent's clonename
            }

            if (resCheckActorExistence(ptr->parentCName)) // If the parent exists
                // Draw the actor in relation to its parent's position
                draw_from(ptr->cName, (p->x + a->x - viewX) * scale,
                                      (p->y + a->y - viewY) * scale, scale);
        }

        prev = ptr;      // Store the current object's pointer
        ptr = ptr->next; // Move on to the next object
    }

    #if RES_DEBUG // If the project is in debug mode
        resDisplayDebugOverlay(); // Display the debug overlay
    #endif
}

// This function draws the static layer
void resDrawStaticLayer(void)
{
    Actor *a = NULL; // Pointer for the actor to draw
    Actor *p = NULL; // Pointer for an actor's parent
    resActorObject *prev = NULL; // Pointer to the previous item in the list
    resActorObject *ptr = resController.staticActorListHead; // List iterator pointer

    int viewX = resController.pView->x; // Get view's x coordinate
    int viewY = resController.pView->y; // Get view's y coordinate
    double scale = resController.scale; // Get the scale for drawing the actor

    erase(0, 0, 0, 1); // Clear the canvas

    while (ptr != NULL) // Traverse through the list
    {
        if (!resCheckActorExistence(ptr->cName)) // If the actor doesn't exist
        {
            // Get next object's address before the current object is deleted
            resActorObject *temp = ptr->next;

            resDeleteActorObjectByPointer(ptr, prev); // Delete current actor object
            ptr = temp;                       // Move on to the next object
            continue; // There's no actor to draw, so skip the rest of this cycle
        }

        a = ptr->actor; // Get the actor pointer

        if (!a->resHasParent) // If the actor doesn't have a parent
            // Draw the actor
            draw_from(ptr->cName, (a->x - viewX) * scale,
                                  (a->y - viewY) * scale, scale);
        else // If the actor has a parent
        {
            // If the object's parent's clonename is already stored,
            // use that to acquire a pointer to the parent
            if (strlen(ptr->parentCName) > 0) p = getclone(ptr->parentCName);
            else
            {
                p = getclone(a->resParentName);             // Get the pointer to the parent
                strcpy(ptr->parentCName, a->resParentName); // Store the parent's clonename
            }

            if (resCheckActorExistence(ptr->parentCName)) // If the parent exists
                // Draw the actor in relation to its parent's position
                draw_from(ptr->cName, (p->x + a->x - viewX) * scale,
                                      (p->y + a->y - viewY) * scale, scale);
        }

        prev = ptr;      // Store the current object's pointer
        ptr = ptr->next; // Move on to the next object
    }

    resController.updateStaticLayer = RES_FALSE; // Lower the static layer update flag
}

// This function sends an activation event to the static layer canvas
void resSendStaticLayerActivationEvent(void)
{
    SendActivationEvent("resStaticLayerCanvas");
}

// This function handles the static layer canvas' activation events
void resHandleStaticLayerActivationEvent(void)
{
    resDrawStaticLayer();
}

// An array defining the 5x5 pixel font to be used for printing text on the debug overlay.
// First five numbers are the first row of pixels, next five are the second one, etc.
// 1 means that the pixel should be drawn and 0 that it shouldn't
char resDebugFont[RES_DEBUG_FONT_CHAR_COUNT][RES_DEBUG_FONT_DOT_COUNT] =
{
    "0010000100001000000000100", "0101001010000000000000000", "0101011111010101111101010", // !"#
    "0111110100011100010111110", "1000100010001000100010001", "0010001010011001001001101", // $%&
    "0010000100000000000000000", "0001000100001000010000010", "0100000100001000010001000", // '()
    "0010011111011100101000000", "0010000100111110010000100", "0000000000000000001000100", // *+,
    "0000000000011100000000000", "0000000000000000000000100", "0000100010001000010001000", // -./
    "0111010011101011100101110", "0010001100001000010000100", "0111010001001100100011111", // 012
    "1111000001001100000111110", "0011001010100101111100010", "1111110000111100000111110", // 345
    "0111010000111101000101110", "1111100001000100010000100", "0111010001011101000101110", // 678
    "0111010001011110000101110", "0000000100000000010000000", "0000000100000000010001000", // 9:;
    "0001000100010000010000010", "0000011111000001111100000", "0100000100000100010001000", // <=>
    "0111010001001100000000100", "0111010011101011001101000",                              // ?@

    "0111010001100011111110001", "1111010010111111000111111", "0111110000100001000001111", // ABC
    "1111010001100011000111110", "1111110000111001000011111", "1111110000111001000010000", // DEF
    "0111010000101111000101110", "1000110001111111000110001", "0010000100001000010000100", // GHI
    "0000100001000011000101110", "1000110010111001001010001", "1000010000100001000011110", // JKL
    "1000111011101011000110001", "1000111001101011001110001", "0111010001100011000101110", // MNO
    "1111010001100011111010000", "0111010001101010111000010", "1111010001100011111010001", // PQR
    "0111110000011100000111110", "1111100100001000010000100", "1000110001100011000101110", // STU
    "1000110001010100101000100", "1000110001101011010101010", "1000101010001000101010001", // VWX
    "1000101010001000010000100", "1111100010001000100011111", "0011000100001000010000110", // YZ[
    "0100000100000100001000001", "0110000100001000010001100", "0010001010100010000000000", // \]^
    "0000000000000000000011111", "0010000010000000000000000",                              // _`

    "0000000111010010101100101", "0100001000011100100101110", "0000000111010000100000111", // abc
    "0000100001001110100100111", "0000000110010100110000111", "0001000101001000111000100", // def
    "0000000011001010001101110", "0100001000011100100101001", "0010000000001000010000100", // ghi
    "0001000000000100101000100", "0100001010011000101001001", "0010000100001000010000010", // jkl
    "0000011010101011010110101", "0000001110010010100101001", "0000000110010010100100110", // mno
    "0000001110010010111001000", "0000000111010010011100001", "0000001010011010100001000", // pqr
    "0000000011001000001001100", "0010001110001000010100010", "0000001001010010100100110", // stu
    "0000001001010010101000100", "0000010001101011010101010", "0000001001001100011001001", // vwx
    "0000001001010100010001000", "0000001111000100010001111", "0011000100011000010000110", // yz{
    "0010000100001000010000100", "0110000100001100010001100", "0000001000101010001000000"  // |}~
};

// This function draws the debug overlay that shows the following information:
// - Resolution information (old & new resolution)
// - Memory currently allocated for the system (in kibibytes (KiB), 1 kibibyte = 1024 bytes)
// - Amount of actors currently on the dynamic actor list
// - Amount of actors currently on the static actor list
// - Top actor for every mouse button held down. In practice this means only right, left and
//     middle mouse buttons, as mouse wheel up and down can't be held down
// - All clicked actors for every mouse button held down. Again, only right, left and middle
//     buttons actually show any values, as mouse wheel up and down can't be held down
void resDisplayDebugOverlay(void)
{
    int i, j; // Iterator variables

    int cNum; // Current character's ASCII value converted to the corresponding font array entry

    static unsigned short int mode = RES_DEBUG_SETTINGS;

    int useLabelColor = 0; // Whether label coloring (green) is currently in use or not
    int useErrorColor = 0; // Whether error coloring (red) is currently in use or not
    int size = RES_DEBUG_FONT_DOT_SIZE; // Pen size for putpixel
    int bgPenSize = RES_DEBUG_FONT_WIDTH * size + 2; // Pen size for drawing the background
    int startX = RES_DEBUG_FONT_WIDTH; // Horizontal margin from the right edge of the screen
    int startY = RES_DEBUG_FONT_WIDTH; // Vertical margin from the top edge of the screen
    int xOffset = 0; // The x coordinate offset from the text start for the current character
    int yOffset = 0; // The y coordinate offset from the text start for the current character

    enum resMouseButtonsEnum buttonNum; // Mouse button array iterator variable

    int debugStringOverflow = 0; // This is set to 1 if the debug string was overflown
    static char debugString[RES_DEBUG_PRINT_MAX_LENGTH]; // The string to be printed
    char temp[50] = ""; // Temporary string for storing an actor's clonename
    char tempDActorNamesString[RES_DEBUG_PRINT_MAX_LENGTH] = ""; // Dynamic actor names list
    char tempSActorNamesString[RES_DEBUG_PRINT_MAX_LENGTH] = ""; // Static actor names list

    resActorObject *ptr; // The list iterator pointer

    static int keyStates[4];   // Stores the states of the keys used to control the overlay
    char *key = GetKeyState(); // Used for updating the stored states of the control keys

    if (key[KEY_F9] && !keyStates[0]) // If key was just pressed down
    {
        mode ^= RES_DEBUG_DRAW_OVERLAY; // Toggle overlay on / off
        keyStates[0] = 1;               // Set key as pressed to avoid toggling repeatedly
    }
    else if (!key[KEY_F9]) keyStates[0] = 0; // On key release, set key as not pressed

    if (key[KEY_F10] && !keyStates[1]) // If key was just pressed down
    {
        mode ^= RES_DEBUG_LIST_DYNAMIC; // Toggle listing dynamic actors' names on / off
        keyStates[1] = 1;               // Set key as pressed to avoid toggling repeatedly
    }
    else if (!key[KEY_F10]) keyStates[1] = 0; // On key release, set key as not pressed

    if (key[KEY_F11] && !keyStates[2]) // If key was just pressed down
    {
        mode ^= RES_DEBUG_LIST_STATIC; // Toggle listing static actors' names on / off
        keyStates[2] = 1;              // Set key as pressed to avoid toggling repeatedly
    }
    else if (!key[KEY_F11]) keyStates[2] = 0; // On key release, set key as not pressed

    if (key[KEY_F12] && !keyStates[3]) // If key was just pressed down
    {
        mode ^= RES_DEBUG_LIST_CLICKED; // Toggle listing clicked actors' names on / off
        keyStates[3] = 1;               // Set key as pressed to avoid toggling repeatedly
    }
    else if (!key[KEY_F12]) keyStates[3] = 0; // On key release, set key as not pressed

    // Only update debugString every # frames, where # = RES_DEBUG_UPDATE_FREQ
    if (frame % RES_DEBUG_UPDATE_FREQ == 0){
    // Print system info, overlay controls, resolution info and memory usage to debugString
    sprintf(debugString,
        "%s%.1f%s\n\n%s%s\n%s%s\n%s%s\n%s%s\n\n%s: %4i x %4i\n%s:   %4i x %4i\n%s:    %7.02f KiB",
            "$RESOLUTIONARY // Version ", RES_VERSION_NUMBER, " // lcl 2017 - 2018$",
            "$F9$  draw overlay ", (mode & RES_DEBUG_DRAW_OVERLAY)?"on":"off",
            "$F10$ list dynamic ", (mode & RES_DEBUG_LIST_DYNAMIC)?"on":"off",
            "$F11$ list static  ", (mode & RES_DEBUG_LIST_STATIC)?"on":"off",
            "$F12$ list clicked ", (mode & RES_DEBUG_LIST_CLICKED)?"on":"off",
            "$Original resolution$",
            resController.originalScreenWidth, resController.originalScreenHeight,
            "$Scaled resolution$",
            resController.scaledScreenWidth, resController.scaledScreenHeight,
            "$Memory allocated$", (double)(resController.debugAllocatedByteCount / 1024.0));

    ptr = resController.dynamicActorListHead; // Set list iterator to dynamic actor list head
    resController.debugDynamicCount = 0;      // Reset dynamic actor count variable to 0
    sprintf(tempDActorNamesString, "\n\n$Dynamic actor list$:"); // Initialize string

    while (ptr != 0) // Traverse through the dynamic actor list
    {
        // If dynamic actor name listing is on, print actor names to string,
        // # names per line, where # = RES_DEBUG_NAMES_PER_LINE
        if (mode & RES_DEBUG_LIST_DYNAMIC)
        {
            int changeLine =
                (resController.debugDynamicCount % (int)max(1, RES_DEBUG_NAMES_PER_LINE) == 0);
            sprintf(temp, "%s%s", (changeLine)?"\n ":" ", ptr->cName);
            resSafeStrcat(tempDActorNamesString, temp, sizeof tempDActorNamesString);
        }

        resController.debugDynamicCount ++; // Increment dynamic actor count variable
        ptr = ptr->next;                    // Move on to the next object
    }

    // If listing is on, but the list is empty
    if (mode & RES_DEBUG_LIST_DYNAMIC && !resController.debugDynamicCount)
        resSafeStrcat(tempDActorNamesString, "\n --- empty ---", sizeof tempDActorNamesString);

    // Print dynamic actor count
    sprintf(temp, "\n$Dynamic actor count$:  %3i", resController.debugDynamicCount);
    if (!debugStringOverflow)
        debugStringOverflow = resSafeStrcat(debugString, temp, sizeof debugString);

    ptr = resController.staticActorListHead; // Set list iterator to static actor list head
    resController.debugStaticCount = 0;      // Reset static actor count variable to 0
    sprintf(tempSActorNamesString, "\n\n$Static actor list$:"); // Initialize string

    while (ptr != 0) // Traverse through the static actor list
    {
        // If static actor name listing is on, print actor names to string,
        // # names per line, where # = RES_DEBUG_NAMES_PER_LINE
        if (mode & RES_DEBUG_LIST_STATIC)
        {
            int changeLine =
                (resController.debugStaticCount % (int)max(1, RES_DEBUG_NAMES_PER_LINE) == 0);
            sprintf(temp, "%s%s", (changeLine)?"\n ":" ", ptr->cName);
            resSafeStrcat(tempSActorNamesString, temp, sizeof tempSActorNamesString);
        }

        resController.debugStaticCount ++; // Increment static actor count variable
        ptr = ptr->next;                   // Move on to the next object
    }

    // If listing is on, but the list is empty
    if (mode & RES_DEBUG_LIST_STATIC && !resController.debugStaticCount)
        resSafeStrcat(tempSActorNamesString, "\n --- empty ---", sizeof tempSActorNamesString);

    // Print static actor count
    sprintf(temp, "\n$Static actor count$:   %3i", resController.debugStaticCount);
    if (!debugStringOverflow)
        debugStringOverflow = resSafeStrcat(debugString, temp, sizeof debugString);

    // Iterate through the mouse button data arrays for every mouse button
    for (buttonNum = RES_MOUSE_LEFT; buttonNum < RES_MOUSE_BUTTONS; buttonNum ++)
    {
        // If the current mouse button has an array of clicked actors stored
        if (resController.mButtonActors[buttonNum])
        {
            char tempName[50];        // String variable for temporarily storing an actor's name
            char tempActorNames[512]; // String variable for temporarily storing all actors' names

            // Print the name of the top actor, or (none), if there's no top actor
            sprintf(tempActorNames, "\n\n$Mouse %i top actor$:\n %s",
                    buttonNum + 1,
                    (strlen(resController.mButtonTopActorCName[buttonNum]) > 0)?
                    resController.mButtonTopActorCName[buttonNum]:"(none)");

            // If listing clicked actors is on
            if (mode & RES_DEBUG_LIST_CLICKED)
            {
                sprintf(tempName, "\n$Mouse %i clicked actors$:", buttonNum + 1);
                resSafeStrcat(tempActorNames, tempName, sizeof tempActorNames);

                // Iterate through the array of clicked actors
                for (i = 0; i < resController.mButtonActorCount[buttonNum]; i ++)
                {
                    int changeLine = (i % (int)max(1, RES_DEBUG_NAMES_PER_LINE) == 0);

                    sprintf(tempName, "%s%s", (changeLine)?"\n ":" ",
                            resController.mButtonActors[buttonNum][i].clonename);

                    // Concatenate tempName to tempActorNames
                    resSafeStrcat(tempActorNames, tempName, sizeof tempActorNames);
                }
            }
            // Concatenate tempActorNames to debugString
            if (!debugStringOverflow)
                debugStringOverflow =
                    resSafeStrcat(debugString, tempActorNames, sizeof debugString);
        }
    }

    // If listing dynamic actors is on, and there's space in debugString, print the list
    if (mode & RES_DEBUG_LIST_DYNAMIC && !debugStringOverflow)
        debugStringOverflow =
            resSafeStrcat(debugString, tempDActorNamesString, sizeof debugString);

    // If listing static actors is on, and there's space in debugString, print the list
    if (mode & RES_DEBUG_LIST_STATIC && !debugStringOverflow)
        debugStringOverflow =
            resSafeStrcat(debugString, tempSActorNamesString, sizeof debugString);
    }

    // If debugString was overflown, show an error message suggesting to increase the string's size
    if (debugStringOverflow)
    {
        const char errText[] =
            "\n\n#Debug string overflown!# Increase $RES_DEBUG_PRINT_MAX_LENGTH$.";
        sprintf(&debugString[RES_DEBUG_PRINT_MAX_LENGTH - sizeof errText], "%s", errText);
    }

    if (!(mode & RES_DEBUG_DRAW_OVERLAY)) return; // If the overlay drawing is off, finish

    // Iterate through debugString character by character
    for (i = 0; i < strlen(debugString); i ++)
    {
        // Convert current character's ASCII value to the corresponding font array entry
        cNum = max(0, min(debugString[i] - (int)'!', RES_DEBUG_FONT_CHAR_COUNT - 1));

        switch (debugString[i])
        {
            case ' ': // If the character is a space, draw the background and increment xOffset
                setpen(0, 0, 0, 0, bgPenSize);
                putpixel(startX + ((xOffset * RES_DEBUG_FONT_SPACING) * size) + size * 2,
                         startY + yOffset + size * 2);
                xOffset ++;
            break;

            case '\n': // If the character is a newline character
                yOffset += RES_DEBUG_FONT_LINE_SPACING * size; // Increment yOffset
                xOffset = 0;                                   // Reset xOffset to 0
            break;

            case '$': // If the character is a non-escaped label marker, toggle label coloring
                if (i < RES_DEBUG_PRINT_MAX_LENGTH && debugString[i + 1] != '$')
                {
                    useLabelColor = !useLabelColor;
                    break;
                }
                else i ++; // Character was escaped ($$), so print it

            case '#': // If the character is a non-escaped error marker, toggle error coloring
                if (debugString[i] == '#' &&
                    i < RES_DEBUG_PRINT_MAX_LENGTH && debugString[i + 1] != '#')
                {
                    useErrorColor = !useErrorColor;
                    break;
                }
                else if (debugString[i] == '#') i ++; // Character was escaped (##), so print it

            default: // All other cases
                setpen(0, 0, 0, 0, bgPenSize);
                putpixel(startX + ((xOffset * RES_DEBUG_FONT_SPACING) * size) + size * 2,
                         startY + yOffset + size * 2); // Draw the background

                if (!useLabelColor && !useErrorColor)
                    setpen(255, 255, 255, 0, size); // Set pen for drawing a normal character
                else if (useLabelColor)
                    setpen(0, 255, 0, 0, size);     // Set pen for drawing a label character
                else if (useErrorColor)
                    setpen(255, 0, 0, 0, size);     // Set pen for drawing an error character

                // Iterate through the font array's entry for the current character
                for (j = 0; j < RES_DEBUG_FONT_DOT_COUNT - 1; j ++)
                {
                    if (resDebugFont[cNum][j] == '1') // If 1 was encountered
                        // Draw a dot to the correct position
                        putpixel(startX + ((xOffset * RES_DEBUG_FONT_SPACING) +
                                 (j % RES_DEBUG_FONT_WIDTH)) * size,
                                 startY + yOffset + (j / RES_DEBUG_FONT_WIDTH) * size);
                }

                xOffset ++; // Increment xOffset
            break;
        }
    }
}



// ---------------- Resolutionary's internal functions for miscellaneous tasks --------------- //

// This function updates Resolutionary's mouse position. The function also updates
// the variables resXmouse and resYmouse that should be used for reading mouse position
// instead of the usual xmouse and ymouse
void resUpdateMouse(void)
{
    Actor *pMouse = getclone("resMouse.0"); // Get the mouse actor pointer

    // Set the mouse actor's screen position in relation to the actual mouse position
    // on the scaled game view. Limit the position to an area that is the size of the
    // original resolution. Update the resXmouse and resYmouse values accordingly
    pMouse->xscreen = resXmouse =
        resLimit((xmouse - resController.mouseXOffset) / (double)resController.scale,
                 0, resController.originalScreenWidth);
    pMouse->yscreen = resYmouse =
        resLimit((ymouse - resController.mouseYOffset) / (double)resController.scale,
                 0, resController.originalScreenHeight);
}

// This function updates the variable that holds the state of the given mouse button when the
// button is pressed down, and sends an activation event to all actors found in
// the current mouse actor position. The function also finds the actor with highest z depth
// at the mouse position, this information is used for allowing the system to simulate
// Game Editor's normal mouse behavior, where only the top actor can be clicked
// mButtonNumber - the mouse button to update, must be one of the following:
//     - RES_MOUSE_LEFT
//     - RES_MOUSE_RIGHT
//     - RES_MOUSE_MIDDLE
//     - RES_MOUSE_WHEEL_UP
//     - RES_MOUSE_WHEEL_DOWN
// returns: 0 on success, 1 if no actors in collision, 2 if memory allocation failed
int resUpdateMouseButtonDown(enum resMouseButtonsEnum mButtonNumber)
{
    int i;         // Array iterator variable
    int count;     // Count of actors in collision
    Actor *actors = NULL; // A pointer for the array of actors in collision

    // Set the mouse button's state to pressed
    resController.mButtonState[mButtonNumber] = RES_MOUSE_DOWN;

    // Get the actors currently in collision with the mouse actor, and their count
    actors = getAllActorsInCollision("resMouse.0", &count);

    // If there's no actors in collision with the mouse actor
    if (!actors) return 1; // Finish

    // If there currently is an array of actors stored for the mouse button, it needs to be emptied
    if (resController.mButtonActors[mButtonNumber])
    {
        #if RES_DEBUG // If the project is in debug mode
            // Update the system's memory usage variable
            resController.debugAllocatedByteCount -=
                resController.mButtonActorCount[mButtonNumber] * sizeof *actors;
        #endif

        free(resController.mButtonActors[mButtonNumber]);   // Free the memory
        resController.mButtonActors[mButtonNumber] = NULL;  // Set the pointer to NULL
        resController.mButtonActorCount[mButtonNumber] = 0; // Set the count to 0
    }

    // Reset the variable used to store the top actor's name
    strcpy(resController.mButtonTopActorCName[mButtonNumber], "");

    // Allocate memory for the array of actors in the current mouse actor position
    resController.mButtonActors[mButtonNumber] = malloc(count * sizeof *actors);
    resController.mButtonActorCount[mButtonNumber] = count; // Store the count of actors

    if (resController.mButtonActors[mButtonNumber]) // If the memory allocation succeeded
    {
        // Copy the contents of the array returned by getAllActorsInCollision to
        // the allocated chunk of memory
        memcpy(resController.mButtonActors[mButtonNumber], actors, count * sizeof *actors);

        #if RES_DEBUG // If the project is in debug mode
            // Update the system's memory usage variable
            resController.debugAllocatedByteCount += count * sizeof *actors;
        #endif
    }
    else // If the memory allocation failed
        return 2; // Finish

    resController.activeButton = mButtonNumber; // Set the mouse button as the active one

    // Of the clicked actors, find the one with the highest z depth. To achieve this, start
    // from the end of the array and iterate through it backwards until a valid actor (any other
    // than resActorDetector) has been found or the beginning of the array is reached
    for (i = resController.mButtonActorCount[mButtonNumber] - 1; i >= 0; i --)
    {
        // If the actor is not resActorDetector
        if (strcmp(resController.mButtonActors[mButtonNumber][i].clonename,
            "resActorDetector.0") != 0)
        {
            // Get the name of the top actor for the mouse button in question
            strcpy(resController.mButtonTopActorCName[mButtonNumber],
                resController.mButtonActors[mButtonNumber][i].clonename);
            break; // Exit the loop
        }
    }

    // Iterate through the array of actors and send an activation event to each of them
    for (i = 0; i < resController.mButtonActorCount[mButtonNumber]; i ++)
    {
        SendActivationEvent(resController.mButtonActors[mButtonNumber][i].clonename);
    }

    return 0; // Finish
}

// This function updates the variable that holds the state of the given mouse button when the
// button is released, and sends an activation event to all actors that were found in
// the mouse actor's position when the mouse button was pressed down
// mButtonNumber - the mouse button to update, must be one of the following:
//     - RES_MOUSE_LEFT
//     - RES_MOUSE_RIGHT
//     - RES_MOUSE_MIDDLE
//     - RES_MOUSE_WHEEL_UP
//     - RES_MOUSE_WHEEL_DOWN
void resUpdateMouseButtonUp(enum resMouseButtonsEnum mButtonNumber)
{
    int i; // Array iterator variable

    // Set the mouse button's state to not pressed
    resController.mButtonState[mButtonNumber] = RES_MOUSE_UP;
    resController.activeButton = mButtonNumber; // Set the mouse button as the active one

    // If a top actor exists
    if (strlen(resController.mButtonTopActorCName[mButtonNumber]) > 0)
        // Send activation event to the top actor
        SendActivationEvent(resController.mButtonTopActorCName[mButtonNumber]);

    // Reset the variable used to store the top actor's name
    strcpy(resController.mButtonTopActorCName[mButtonNumber], "");

    // If there is an array actors stored for the mouse button
    if (resController.mButtonActors[mButtonNumber])
    {
        // Iterate through the array of actors and send an activation event to each of them
        for (i = 0; i < resController.mButtonActorCount[mButtonNumber]; i ++)
        {
            SendActivationEvent(resController.mButtonActors[mButtonNumber][i].clonename);
        }

        #if RES_DEBUG // If the project is in debug mode
            // Update the system's memory usage variable
            resController.debugAllocatedByteCount -=
                (resController.mButtonActorCount[mButtonNumber] *
                 sizeof *resController.mButtonActors[mButtonNumber]);
        #endif

        // Empty the array of actors
        free(resController.mButtonActors[mButtonNumber]);   // Free the memory
        resController.mButtonActors[mButtonNumber] = NULL;  // Set the pointer to NULL
        resController.mButtonActorCount[mButtonNumber] = 0; // Set the count to 0
    }
}

// This function can be used to concatenate a string to another without having to worry about
// overflowing the destination string. If there's not enough space in the destination string,
// the function concatenates as many characters from the beginning of the source string as there
// is space for before the string has to be null terminated. Rest of the source string is cropped
// out of the concatenation.
// destination - pointer to the string to concatenate to
// source - pointer to the string to concatenate
// maxLen - the length of the longest string the array can contain
// returns: 0 on success, 1 if some of the source string had to be cropped from concatenation
int resSafeStrcat(char *destination, const char *source, int maxLen)
{
    int dLen = strlen(destination);                   // Current length of destination string
    int sLen = strlen(source);                        // Current length of source string
    int catLen = sLen - (dLen + sLen - (maxLen - 1)); // Calculate how many characters at most
                                                      // can be concatenated to destination

    strncat(destination, source, catLen); // Concatenate as much as possible

    return (catLen < sLen); // Return 1 if some of the source string had to be left out
}

// This function limits a value between given boundaries
// value - the value that is to be limited
// minVal - the minimum value the variable can have
// maxVal - the maximum value the variable can have
// returns: the original value if it is between the limiting values,
//     otherwise returns the corresponding limit value
double resLimit(double value, double minVal, double maxVal)
{
    return max(min(minVal, maxVal), min(value, max(maxVal, minVal)));
}

// This function disables all mouse events of a given actor
// actorName - name of the actor that should have its mouse events disabled
void resDisableMouseEvents(const char *actorName)
{
    EventDisable(actorName, EVENTMOUSEBUTTONDOWN);
    EventDisable(actorName, EVENTMOUSEBUTTONUP);
    EventDisable(actorName, EVENTMOUSEENTER);
    EventDisable(actorName, EVENTMOUSELEAVE);
}

// This function enables all mouse events of a given actor
// actorName - name of the actor that should have its mouse events enabled
void resEnableMouseEvents(const char *actorName)
{
    EventEnable(actorName, EVENTMOUSEBUTTONDOWN);
    EventEnable(actorName, EVENTMOUSEBUTTONUP);
    EventEnable(actorName, EVENTMOUSEENTER);
    EventEnable(actorName, EVENTMOUSELEAVE);
}

// This function checks if an actor exists or not
// actorName - name or clonename of the actor to be checked
// returns: 1 if actor exists, 0 if not
int resCheckActorExistence(const char *actorName)
{
    return (getclone(actorName)->cloneindex != -1);
}

// This function checks if an actor has already been added
// to either one of the drawable actor lists
// a - pointer to the actor that is to be checked
// returns: 1 if actor is on a list, 0 if not
int resActorIsListed(Actor *a)
{
    return a->resIsOnActorObjectList;
}



// ---------------------------- Resolutionary by lcl / 2017 - 2018 --------------------------- //