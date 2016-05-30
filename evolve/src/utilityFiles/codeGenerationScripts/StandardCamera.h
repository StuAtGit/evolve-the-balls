#ifndef STU_StandardCamera_H
#define STU_StandardCamera_H


#define DO_DEBUG true

using namespace std;
using namespace boost;

/**TODO: insert class comments*/
class StandardCamera
{
public:
    StandardCamera( SceneManager* sceneManager, RenderWindow* renderWindow );
    /**
    *
    */
    bool foward( bool printDebug = DO_DEBUG );
    /**
    *
    */
    bool backward( bool printDebug = DO_DEBUG );
    /**
    *
    */
    bool left( bool printDebug = DO_DEBUG );
    /**
    *
    */
    bool right( bool printDebug = DO_DEBUG );
    /**
    *
    */
    bool upward( bool printDebug = DO_DEBUG );
    /**
    *
    */
    bool downward( bool printDebug = DO_DEBUG );
    /**
    *
    */
    bool setSpeed( double speed,bool printDebug = DO_DEBUG );
    /**
    *
    */
    bool setRotateSpeed( double speed,bool printDebug = DO_DEBUG );
    /**
    *
    */
    bool rotate( double x,double y,bool printDebug = DO_DEBUG );
    /**
    *
    */
    map<string,Vector3> getSelected( double clickX, double clickY,bool printDebug = DO_DEBUG );
private:
};

#endif
