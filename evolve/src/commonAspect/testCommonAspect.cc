#include "CommonAspect.h"

int main()
{
    CommonAspect cac;
    cac.setNotify( true );
    Logger* logger = NULL;
    cac.getLogger( logger );
    logger->print( "test..." );
}
