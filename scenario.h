#ifndef SCENARIO_H
#define SCENARIO_H

#include <functional>
#include <ui_mainwindow.h>

namespace Ui {
    class Scenario {
        public:

            virtual ~Scenario() = default;

            void addStep(std::function<void()> step);
            void executeNextStep(Ui::MainWindow* ui);

            Scenario();

        protected:
            std::vector<std::function<void()>> steps;
            size_t currentStep = 0;
    };
}


#endif // SCENARIO_H
