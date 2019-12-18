class Menu {
  public:
    int id;
    int parent;
    String lable;

  public:
    Menu(int id, int parent, String lable);
    void execute();
};

void Menu::execute() {
  Serial.println(lable);
}
Menu::Menu(int idx, int parentx, String lablex) {
  id = idx;
  parent = parentx;
  lable = lablex;
}
