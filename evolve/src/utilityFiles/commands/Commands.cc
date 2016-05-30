#ifndef COMMANDS_STU_CC
#define COMMANDS_STU_CC
#include "Commands.h"

Commands* Commands::instance = NULL;
Commands::Commands()
{
    this->createCommand = "create";
    this->initCommand = "init";
    this->startCommand = "start";
    this->stopCommand = "stop";
    this->printCommand = "print";
    this->destroyCommand = "destroy";
    this->makeCopyCommand = "makeCopy";
    this->getSystemCommand = "getSystem";
    this->getMaxCommand = "getMax";
    this->getMinCommand = "getMin";
    this->getIdsCommand = "getIds";
    this->getModeCommand = "getMode";
    this->getSystemLengthCommand = "getStringSystemLength";
    this->getProductionRulesCommand = "getProductionRules";
    this->getOptionsCommand = "getOptionsCommand";
    this->getMutationProbabilityCommand = "getMutationProbability";
    this->setMaxCommand = "setMax";
    this->setMinCommand = "setMin";
    this->setSystemCommand = "setSystem";
    this->setModeCommand = "setMode";
    this->setProductionCommand = "setProduction";
    this->deleteProductionCommand = "deleteProduction";
    this->setMutationProbabilitiesCommand = "setMutationProbabilities";
    this->setMutationProbabilityCommand = "setMutationProbability";
    this->setOptionsCommand = "setOptions";
    this->deleteOptionCommand = "deleteOption";
    this->loadCommand = "load";
    this->saveCommand = "save";
    this->runCommand = "run";
    this->getMutationTypeDescriptionCommand = "getMutationTypeDescription";
    this->crossCommand = "cross";
    this->breedCommand = "breed";
    this->getNewIdCommand = "getNewId";
    this->initFromFileCommand = "initFromFile";
    this->initFromMapCommand = "initFromMap";
    this->reloadCommandHandler = "reloadHandler";
    this->lSystemFileRequest = "Notify: Please send the lsystem file contents now:";
    this->initMapRequest = "Notify: Please send the initialization map now:";
    this->systemIdRequest = "Notify: please send the id of the system now:";
    this->systemCrossMapRequest = "Notify: please crossover map for the systems now:";
    this->optionsCrossMapRequest = "Notify: please crossover map for the options now:";
    this->mutationProbabilitiesCrossMapRequest = "Notify: please crossover map for the mutation probabilities now:";
    this->growthRulesCrossMapRequest = "Notify: please crossover map for the growth rules now:";
    this->systemRequest = "Notify: please send system now:";
    this->mutationProbabilitiesRequest = "Notify: please send mutation probabilities  now. Types supported are:1-4:Format is prob1:1:prob2:2:prob3:3:prob4:4";
    this->mutationProbabilityRequest = "Notify: please send mutation probability for type:";
    this->mutationProbabilityTypeRequest = "Notify: please send the type of mutation your settting:";
    this->optionNumberRequest = "Notify: please send number of options, max option size";
    this->optionsRequest = "Notify: please send options now";
    this->getMaxRequest = "Notify: please send the max length of the system now";
    this->getMinRequest = "Notify: please send the min length of the system now";
    this->getModeRequest = "Notify: please send the new mode of the lsystem as an integer 0 (do nothing), 1 (mutate)  or 2 (grow)";
    this->runIterationRequest = "Notify: please send the number of iteration you wish to perform";
    this->startProductionRequest = "Notify: please send the start production:";
    this->endProductionRequest = "Notify: please send the end production:";
    this->successReply = "Notify: command executed successfully";
    this->failedReply = "Warning: failure executing command";
    this->endMessage = "_END_M";
    this->shutdownMessage = "SHUTDOWN";
    this->createInterpreterIntCommand = "createInterpreter";
    this->loadAttributesIntCommand = "loadAttributes";
    this->saveAttributesIntCommand = "saveAttributes";
    this->getSystemIntCommand = "getSystem";
    this->reloadConfigIntCommand = "reloadConfig";
    this->setAttributeIntCommand = "setAttribute";
    this->getAttributeIntCommand = "getAttribute";
    this->getAttributeDescriptionIntCommand = "getAttributeDescription";
    this->getSupportedAttributesIntCommand = "getSupportedAttributes";
    this->getAttributeNameIntRequest = "Notify: please send the name of the attribute now";
    this->getAttributeValueIntRequest = "Notify: please send the value for the attribute now";
    this->endMessageBlock = "_END_MESSAGE_BLOCK";
    this->invalidId = "_INVALID_ID";
    this->defaultLibId = "DEFAULT_LIB";
}
Commands* Commands::getInstance()
{
    if( instance == NULL )
    {
        instance = new Commands();
    }
    return instance;
}
#endif
