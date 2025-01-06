
#include "scenario.h"

class SimulationController {
public:
    void setScenario(std::unique_ptr<Ui::Scenario> newScenario) {
        this->scenario = std::move(newScenario);
    }

    void executeNextStep(Ui::MainWindow* ui) {
        if (scenario) {
            scenario->executeNextStep(ui);
        } else {
            ui->txtLog->append("No scenario loaded.");
        }
    }

private:
    std::unique_ptr<Ui::Scenario> scenario;
};
