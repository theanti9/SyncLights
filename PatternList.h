class PatternNode {
  public:
    PatternNode(PatternInstruction *instr) {
      instruction = instr;
      next = 0;
    }
    
    PatternNode* getNext() {
      return next;
    }
    
    PatternInstruction* getInstruction() {
      return instruction;
    }
    
    void setNext(PatternNode *_next) {
      next = _next;
    }
    
  private:
    PatternInstruction *instruction;
    PatternNode *next; 
};

class PatternList {
  public:
    PatternList() { first = last = next = NULL; length = pos =  0; }
    void add(PatternInstruction *instr) {
      PatternNode *node = new PatternNode(instr);
      if (first == 0) {
        first = node;
      } else {
        last->setNext(node);
      }
      last = node;
      length++;
    }
    
    PatternNode* getNext() {
      if (pos == length) {
        return 0;
      }
      if (next == 0) {
        if (first == 0) {
         return 0; 
        }
        next = first;
      } else {
        next = next->getNext();
      }
      pos++;
      return next;
    }
    
    void reset() {
      pos = 0;
    }
  private:
    PatternNode *first;
    PatternNode *next;
    PatternNode *last;
    int length, pos;
};
