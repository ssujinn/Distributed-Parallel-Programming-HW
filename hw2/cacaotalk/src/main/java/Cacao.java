import java.util.*;
import java.io.*;

class member {
    public String ID;
    public int chatnum;
    public ArrayList<String> chat;
}

class Login {
    public String ID;

    public void menu() {
        System.out.println("");
        System.out.println("Welcome to CacaoTalk");
        System.out.println("1. Log in");
        System.out.println("2. Exit\n");
    }

    public String login() {
        Scanner sc = new Scanner(System.in);

        System.out.print("cacaotalk> ID: ");
        ID = sc.nextLine();

        return ID;
    }
}

class Chatting {
    public void menu(String name) {
        System.out.println("");
        System.out.println(name + "'s Chatting");
        System.out.println("1. List");
        System.out.println("2. Make");
        System.out.println("3. Join");
        System.out.println("4. Log out\n");
    }

    public void list(ArrayList<String> chat){
        for (int i = 0; i < chat.size(); i++)
            System.out.println(chat.get(i));
        System.out.println("");
    }
}


public class Cacao {
    public static void main(String[] args) throws Exception {
        int sel, num = 0, flag = 0, idx = 0;
        int endflag = 0, reset = 0;
        String ID;
        ArrayList<member> mem = new ArrayList();
        Scanner sc = new Scanner(System.in);
        String line, room, txt;

        BufferedReader br = new BufferedReader(new FileReader("history.txt"));
        line = br.readLine();
        if (line==null) num = 0;
        else {
            num = Integer.parseInt(line);
            while (true) {
                line = br.readLine();
                if (line == null) break;
                member m = new member();
                m.ID = line;

                line = br.readLine();
                if (line == null) break;
                m.chatnum = Integer.parseInt(line);
                m.chat = new ArrayList();

                while (true) {
                    line = br.readLine();
                    if (line == null) break;
                    m.chat.add(line);
                }
                mem.add(m);
            }
        }
        br.close();

        while (true) {
            Login l  = new Login();
            Chatting c = new Chatting();
            idx = 0;

            // Login Window
            l.menu();
            while (true) {
                System.out.print("cacaotalk> ");
                sel = sc.nextInt();

                // Log in
                if (sel == 1) {
                    ID = l.login();
                    flag = 0;
                    for (int i = 0; i < mem.size(); i++){
                        if (mem.get(i).ID.equals(ID)){
                            flag = 1;
                            idx = i;
                            break;
                        }
                    }
                    if (flag == 0) {
                        idx = mem.size();
                        num++;
                        member m = new member();
                        m.ID = ID;
                        m.chatnum = 0;
                        m.chat = new ArrayList();
                        mem.add(m);
                    }
                    break;
                }
                // Exit
                else if (sel == 2) {
                    endflag = 1;
                    System.out.println("Bye.");
                    break;
                }
                else continue;
            }
            if (endflag == 1) break;

            // Chatting Window
            c.menu(mem.get(idx).ID);
            while (true){
                System.out.print("cacaotalk> ");
                sel = sc.nextInt();

                // List
                if (sel == 1){
                    c.list(mem.get(idx).chat);
                }
                // Make
                else if (sel == 2){
                    System.out.print("cacaotalk> Chat room name: ");
                    room = sc.next();

                    flag = 0;
                    for (int i = 0; i < mem.get(idx).chatnum; i++){
                        if (mem.get(idx).chat.get(i).equals(room)){
                            flag = 1;
                            System.out.println(room + " already exists");
                            break;
                        }
                    }
                    if (flag == 0) {
                        mem.get(idx).chatnum++;
                        mem.get(idx).chat.add(room);
                        Producer.genTopic(mem.get(idx).ID, room);
                        //Consumer.Sub(mem.get(idx).ID, room);
                        System.out.println(room + " is created!");
                    }
                }
                // Join
                else if (sel == 3){
                    System.out.print("cacaotalk> Chat room name: ");
                    room = sc.next();

                    flag = 0;
                    for (int i = 0; i < mem.get(idx).chatnum; i++) {
                        if (mem.get(idx).chat.get(i).equals(room)) {
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 0){
                        System.out.println(room + " doesn't exist");
                        continue;
                    }
                    else {
                        // Chat Room Window
                        System.out.println("");
                        System.out.println(room);
                        System.out.println("1. Read");
                        System.out.println("2. Write");
                        System.out.println("3. Reset");
                        System.out.println("4. Exit\n");
                        reset = 0;
                        while (true){
                            System.out.print("cacaotalk> ");
                            sel = sc.nextInt();

                            // Read
                            if (sel == 1){
                                if (reset == 0)
                                    Consumer.Recv(mem.get(idx).ID, room, false);
                                else if (reset == 1) {
                                    Consumer.Recv(mem.get(idx).ID, room, true);
                                    reset = 0;
                                }
                            }
                            // Write
                            else if (sel == 2){
                                System.out.print("cacaotalk> Text: ");
                                txt = sc.nextLine();
                                txt = sc.nextLine();
                                Producer.Send(mem.get(idx).ID, room, txt);
                            }
                            // Reset
                            else if (sel == 3){
                                reset = 1;
                            }
                            // Exit
                            else if (sel == 4){
                                c.menu(mem.get(idx).ID);
                                break;
                            }
                            else continue;
                        }
                    }
                }
                // Log out
                else if (sel == 4) break;
                else continue;
            }
        }

        PrintWriter pw = new PrintWriter("history.txt");
        pw.println(num);
        for (int i = 0; i < mem.size(); i++) {
            pw.println(mem.get(i).ID);
            pw.println(mem.get(i).chatnum);
            for (int j = 0; j < mem.get(i).chat.size(); j++)
                pw.println(mem.get(i).chat.get(j));
        }
        pw.close();

    }
}