#include "scenario.h"
#include <ui_mainwindow.h>


Ui::Scenario::Scenario() {}

void Ui::Scenario::addStep(std::function<void()> step) { this->steps.push_back(step); }
void Ui::Scenario::executeNextStep(Ui::MainWindow* ui) {
    if(currentStep < this->steps.size()) {
        steps[currentStep]();
        currentStep++;
    } else ui->txtLog->append("Simulation done");
}
