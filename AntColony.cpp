//
// Created by 王奕翔 on 2018/1/4.
//

#include <cmath>
#include <cstdlib>
#include <random>
#include <iostream>
#include "AntColony.h"

AntColony::AntColony(int taskCount, int processCount, int antCount, double *transDataVol) {
    this->taskCount = taskCount;
    this->processCount = processCount;
    this->antCount = antCount;
    this->transDataVol = transDataVol;
    initMap();
}

void AntColony::initMap() {
    map = new double[taskCount * processCount];
    init2DArray(map, taskCount, processCount);
}

void AntColony::init2DArray(double *array, int x, int y) {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            *(array + j + i * y) = initPheromones;
        }
    }
}

void AntColony::intAnt() {
    ants = new Ant[antCount];
    for (int i = 0; i < antCount; i++) {
        ants[i].init(taskCount, processCount, transDataVol);
    }
}

void AntColony::run(int iteration) {
    intAnt();
    for (int i = 0; i < iteration; i++) {
        moveAnts();
        return ;
        getBestAnt();
        updatePheromones();
        evaporatePheromones();
    }
}

void AntColony::moveAnts() {
    for (int i = 0; i < this->antCount; ++i) {
        moveAnt(ants[i]);
        std::cout << "Ant #" << i << std::endl;
        ants[i].printTaskSchedule();
        ants[i].printProcessMatch();
        ants[i].printDoneTask();
    }
}

void AntColony::moveAnt(Ant &ant) {
    ant.clear();
    for (int i = 0; i < this->taskCount; ++i) {
        if (i == 0) {
            ant.selectTask(0);
        } else {
            ant.selectTask(getRandTask(ant));
        }
        ant.selectProcess(getRandProcess(ant));
    }
}

void AntColony::getBestAnt() {

}

void AntColony::updatePheromones() {

}

void AntColony::evaporatePheromones() {

}

int AntColony::getRandProcess(Ant &ant) {
    double processProbability[processCount], processProbabilitySum = 0.0;

    for (int i = 0; i < processCount; ++i) {
        processProbability[i] = calculateProbability(getPheromones(i, ant.getCurrentProcess()));
        processProbabilitySum += processProbability[i];
    }

    double targetPoint = getRandom(processProbabilitySum);

    for (int i = 0; i < taskCount; ++i) {
        targetPoint -= processProbability[i];
        if (targetPoint <= 0.0) {
            return i;
        }
    }

    std::cout << "[ERROR] getRandProcess" << std::endl;
    return 0;
}

int AntColony::getRandTask(Ant &ant) {
    double taskProbability[taskCount], taskProbabilitySum = 0.0;
    for (int i = 0; i < taskCount; ++i) {
        if (!ant.canRunTask(i)) {
            taskProbability[i] = 0.0;
            continue;
        }
        taskProbability[i] = calculateProbability(getPheromones(i, ant.getCurrentProcess()));
        taskProbabilitySum += taskProbability[i];
    }
    double targetPoint = getRandom(taskProbabilitySum);
    for (int i = 0; i < taskCount; ++i) {
        targetPoint -= taskProbability[i];
        if (targetPoint <= 0.0) {
            return i;
        }
    }

    std::cout << "[ERROR] getRandTask" << std::endl;
    return 0;
}

double AntColony::calculateProbability(double pheromones) {
    return pow(pheromones, alpha);
}

double AntColony::getPheromones(int taskID, int processID) {
    return *(map + taskID * processCount + processID);
}

double AntColony::getRandom(double fmax) {
    std::random_device rd;
    std::default_random_engine gen = std::default_random_engine(rd());
    std::uniform_real_distribution<double> dis(0.0, fmax);

    return dis(gen);
}
