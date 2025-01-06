#ifndef INODE_H
#define INODE_H

    #include "Frame.h"
    #include <queue>

    struct customPriorityChecker{
        bool operator()(Frame* l, Frame* r) const { return l->getPriority() < r->getPriority(); }
    };

    class INode {
        public:
            INode(DWORD);
            bool  writeInBus();
            bool readFromBus();
            Frame encodeData(DWORD, DWORD, DWORD, BOOL, BYTE);

            DWORD getIndentifier();
            Frame getCurrentFrame();
            std::priority_queue<Frame*, std::vector<Frame*> ,customPriorityChecker> getFrameQueue();

            void insertFrame(Frame* frame) {
                this->frames.push(frame);
            }

            Frame* getTopFrame() {
                if(this->frames.empty()) return nullptr;

                Frame* temp = this->frames.top();
                this->frames.pop();
                this->currentFrame = *temp;
                return temp;
            }

            QString getTopFrameString() {
                Frame* temp = this->frames.top();
                if(temp == NULL) return "No frame available";
                return temp->toPipeStructure();
            }

            void  setFrame(Frame);

            virtual BOOL handleFrame(Frame*, INode* = nullptr) = 0;
            QString toString();

        private:
            DWORD  identifier;
            Frame  currentFrame;
            std::priority_queue<Frame*, std::vector<Frame*>, customPriorityChecker> frames;
    };

#endif // INODE_H
