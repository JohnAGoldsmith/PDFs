### Changing a filename
2 steps: 
1. generate a new filename, and possibly edit it by hand;
2. change the filename of the selected item to that new name

functions involved: 
1. Widget::generate_new_title()
2. Widget::change_onboard_filename(QString new_full_filename); this calls: 
3. Widget::change_filename(QString old_name, QString new_name)
4. In widget.cpp: connect(m_keyCtrlK, SIGNAL(activated()), this, SLOT(change_filename()));

also:
m_generate_new_filename_button = new QPushButton("Generate new filename and key");
 
