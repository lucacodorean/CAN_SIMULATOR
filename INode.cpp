#include "INode.h"
#include "clientnode.h"
#include "rgbclientnode.h"
#include <fstream>
#include <vector>
#include <QMessageBox>

template<typename Base, typename T>
    inline bool instanceof(const T *ptr) {
        return dynamic_cast<const Base*>(ptr) != nullptr;
}

std::fstream BUS("CAN_BUS.pipe", std::ios_base::in);
std::vector<std::string>framesInFile;

int getPriority(std::string frame) {

    reverse(frame.begin(), frame.end());
    char frameC[128] = {0};

    std::strncpy(frameC, frame.c_str(), frame.size());

    char* temp = strtok(frameC, " ");
    return std::stoi(temp);
}

bool sortFrames() {
    int n = framesInFile.size();
    bool swapped;

    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (getPriority(framesInFile[j]) < getPriority(framesInFile[j + 1])) {
                swap(framesInFile[j], framesInFile[j + 1]);
                swapped = true;
            }
        }

        if (!swapped)
            break;
    }

    return true;
}

INode::INode(DWORD identifier) : identifier(identifier){ }

bool validateFrame(Frame frame, const ClientNode* destinationNode) {
    if(instanceof<RGBClientNode>(destinationNode)) return frame.getData() <= 0xFFFFFF;
    if(instanceof<ClientNode>(destinationNode))    return frame.getData() <= 0x07FFFF;

    return false;
}

Frame INode::getCurrentFrame() { return this->currentFrame; }
DWORD INode::getIndentifier()  { return this->identifier;   }
std::priority_queue<Frame*, std::vector<Frame*> ,customPriorityChecker> INode::getFrameQueue() { return this->frames; }

void INode::setFrame(Frame frame) { this->currentFrame = frame; }

bool INode::writeInBus() {
    if(BUS.is_open()) BUS.close();
    if(this->getFrameQueue().empty()) {

        QString errorContext;
        errorContext.append("Node has no available frame. Probably the frame that has been sent had an error.");

        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText(errorContext);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return false;
    }

    framesInFile.clear();
    framesInFile.push_back(this->getTopFrame()->toPipeStructure().toStdString());
    BUS.open("CAN_BUS.pipe", std::ios_base::in);

    std::string buffer;
    while (getline(BUS, buffer)) framesInFile.push_back(buffer);
    BUS.close();

    sortFrames();

    BUS.open("CAN_BUS.pipe", std::ios_base::out);
    std::streampos prev_location = BUS.tellp();
    for(auto frame : framesInFile) BUS<<frame<<std::endl;

    std::streampos current_location = BUS.tellp();
    BUS.close();


    bool ok = current_location-prev_location > 0;

    if(!ok) {
        QString errorContext = "Node ";
        errorContext.append(std::to_string(this->identifier));
        errorContext.append(" failed to write in bus due to a BUS-related problem.");

        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText(errorContext);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }

    return ok;
}

void jumpLines(int index) {
    std::string buffer;
    for(int i = 0; i<index-1; i++) getline(BUS, buffer);
    return;
}

bool INode::readFromBus() {
    if(BUS.is_open()) BUS.close();
    BUS.open("CAN_BUS.pipe", std::ios_base::in);

    Frame* temp = new Frame();
    bool ok = false, done = false;
    int index = 0;
    std::string line;

    DWORD destinationNode;

    do {
        std::getline(BUS, line);
        index++;

        if(line.size() == 0) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Error");
            msgBox.setText("Node attempting to read from the BUS when there are no frames available for it.");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
            BUS.close();
            delete temp;
            return false;
        }

        char lineC[128] = {0};
        std::strncpy(lineC, line.c_str(), line.size());

        char* parser = std::strtok(lineC, " ");
        DWORD originNode      = std::stoi(parser);                  parser = std::strtok(NULL, " ");
              destinationNode = std::stoi(parser);                  parser = std::strtok(NULL, " ");
        DWORD data            = std::stoull(parser, nullptr, 16);   parser = std::strtok(NULL, " ");

        DWORD ACK             = std::stoi(parser);                  parser = std::strtok(NULL, " ");
        DWORD priority        = std::stoi(parser);                  parser = std::strtok(NULL, " ");

        temp = new Frame(originNode, destinationNode, 0x1, data, 0x11, priority);

        std::string tempString = "R: " + temp->toPipeStructure().toStdString();
        bool written = false;

        if(data == 0xFF && ACK == 1) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Acknowledgement");
            msgBox.setText("AL-node has received ACK_BIT 1 and data 0xFF which represents success.");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);

            if(priority != 0x00) {
                CAN_NODE* myNode = dynamic_cast<CAN_NODE*>(this);
                myNode->updateFrameHistory(tempString);
            }

            written = true;
            delete temp;
            ok = true;
            done = true;
            msgBox.exec();
            break;
        }

        if(ACK == 0 && priority == 0x09 || destinationNode != this->getIndentifier()) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Error");
            msgBox.setText("Attempting to send a frame to a node that has a different destination.");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();

            break;
        }

        if(destinationNode == this->getIndentifier() && !written) {
            if(priority != 0x00) {
                CAN_NODE* myNode = dynamic_cast<CAN_NODE*>(this);
                myNode->updateFrameHistory(tempString);
            }
            ok = true;
            break;
        }

    } while(TRUE);
    BUS.close();

    if(ok) {
        this->setFrame(*temp);
        this->insertFrame(temp);

        framesInFile[index-1][framesInFile[index-1].size() -1] = '0';
        sortFrames();

        BUS.open("CAN_BUS.pipe", std::ios_base::out);

        for(auto line : framesInFile) BUS<<line<<std::endl;
        BUS.close();
    }

    else delete temp;

    return ok || done;
}

Frame INode::encodeData(DWORD data, DWORD originNode, DWORD destinationNode, BOOL ACK, BYTE priority = 1) {
    return Frame(originNode, destinationNode, ACK, data, 0x11, priority);
}

QString INode::toString() {
    QString data;
    if(this->currentFrame.getPriority() > 9) {
        data.append("This node doesn't have any frame.");
        return data;
    }

    data = "Node identifier: ";
    data.append(std::to_string(this->identifier));
    data.append(this->currentFrame.toString());
    return data;
}
