typedef void (*CallBackFunction)(int id);
typedef void (*CallBackRotaryFunction)(int id);

class Menu {
public:
  int id;
  int parent;
  CallBackFunction callBack;
  CallBackRotaryFunction callBackRotary;
  bool isEntry;
  bool disableScroll;
  String lable;

public:
  Menu(int id, int parent, CallBackFunction callBack, CallBackRotaryFunction callBackRotary, bool isEntry, bool disableScroll, String lable);
  void execute();
  void executeRotary();
};

void Menu::execute() {
   callBack(id);
}
void Menu::executeRotary() {
   callBackRotary(id);
}


Menu::Menu(int idx, int parentx, CallBackFunction callBackx, CallBackRotaryFunction callBackRotaryx, bool isEntryx, bool disableScrollx, String lablex) {
  id = idx;
  parent = parentx;
  callBack = callBackx;
  callBackRotary = callBackRotaryx;
  isEntry = isEntryx;
  disableScroll = disableScrollx;
  lable = lablex;

}

void noFunc(int id) {
}
