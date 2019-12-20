typedef void (*CallBackFunction)(int id);

class Menu {
public:
  int id;
  int parent;
  CallBackFunction callBack;
  bool isEntry;
  String lable;

public:
  Menu(int id, int parent, CallBackFunction callBack, bool isEntry, String lable);
  void execute();
};

void Menu::execute() {
   callBack(id);
}

Menu::Menu(int idx, int parentx, CallBackFunction callBackx, bool isEntryx, String lablex) {
  id = idx;
  parent = parentx;
  callBack = callBackx;
  isEntry = isEntryx;
  lable = lablex;

}

void noFunc(int id) {
}
