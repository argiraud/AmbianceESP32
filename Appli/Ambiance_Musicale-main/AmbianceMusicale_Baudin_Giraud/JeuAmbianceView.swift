//
//  HomeScreenView.swift
//  AmbianceMusicale_Baudin_Giraud
//
//  Created by Maxime Baudin on 09/01/2021.
//


import SwiftUI
import Firebase
import FirebaseFirestore

struct JeuAmbianceView: View {
    @Binding var titre: String
    @Binding var couleur: String
    @Binding var id:String
    
    @State var show = false
    @State var status = UserDefaults.standard.value(forKey: "status") as? Bool ?? false
    @ObservedObject var datas = observer()
    @State var afficheMenu: Bool = false
    
    
    var body: some View{
        
        NavigationView{
            
            VStack{
                
                if self.afficheMenu{
                    HomeScreenView()
                }
                else {
                    
                    if (titre != ""){
                        AfficheJeu(titleAmbient: self.$titre, idJeu: self.$id,colorJeu: self.$couleur, afficheMenu: self.$afficheMenu)
                    } else{
                        CreerJeu(afficheMenu: self.$afficheMenu)
                    }
                        
                    
                        
                    }
                }
            }
            .navigationBarTitle("")
            .navigationBarHidden(true)
            .navigationBarBackButtonHidden(true)
    }
}

struct test {
    
    @State var title: String = ""
    
}

/**struct JeuAmbianceView_Previews: PreviewProvider {
    static var previews: some View {
        JeuAmbianceView(titre: , couleur: "")
    }
}**/


struct CreerJeu: View {
    @State var backgroundColor = Color(.systemBackground)
    @State var titleAmbient = ""
    @State private var selectedColor = Color.red
    @Binding var afficheMenu: Bool
    var color = ["Bleu", "Rouge", "Vert", "Jaune"]
    @State private var selectedCol = 0
    
    var body: some View{
        
        VStack{
            
            Button(action: {
                
                self.retourMenu()
            }) {
                
                Text("Retour Menu")
                    .foregroundColor(.black)
                    .padding(.vertical)
                    .frame(width: UIScreen.main.bounds.width - 50)
            }
            .background(Color.white)
            .cornerRadius(10)
            .padding(.top, 25)
            
            Text("Nom de l'ambiance")
                .font(.title2)
                .fontWeight(/*@START_MENU_TOKEN@*/.bold/*@END_MENU_TOKEN@*/)
                .foregroundColor(Color.white)
                .padding(.top, 35)
            
            TextField("Titre", text: self.$titleAmbient)
                .autocapitalization(.none)
                .padding()
                .background(RoundedRectangle(cornerRadius: 4).stroke(self.titleAmbient != "" ? Color("Color") : Color.white, lineWidth: 2))
                .padding(.top, 25)
        
            
            Text("Choix de la couleur")
                .font(.title2)
                .fontWeight(/*@START_MENU_TOKEN@*/.bold/*@END_MENU_TOKEN@*/)
                .foregroundColor(Color.white)
                .padding(.top, 35)
            
            Picker(selection: $selectedCol, label: Text("Color")) {
                                    ForEach(0 ..< color.count) {
                                        Text(self.color[$0])

                                    }
            }
            
            Button(action: {
                self.addData()
            }) {
                
                Text("Création Jeu")
                    .foregroundColor(.black)
                    .padding(.vertical)
                    .frame(width: UIScreen.main.bounds.width - 50)
            }
            .background(Color.white)
            .cornerRadius(10)
            .padding(.top, 25)
            
        }
    }
    
    func addData(){
        
        let db = Firestore.firestore()
        let ambiance = db.collection("ambiance").document()
        
        ambiance.setData(["id":ambiance.documentID,"titre": self.titleAmbient,"couleur":self.color[selectedCol],"proprietaire":Auth.auth().currentUser?.email ?? "all"]) { (err) in
            
            if err != nil{
                
                print((err?.localizedDescription)!)
                return
            }
            print("success")
            
        }
        
        afficheMenu = true
        
    }
    
    func retourMenu(){
        
        afficheMenu = true
    }
    
}


struct AfficheJeu: View {
    @State var backgroundColor = Color(.systemBackground)
    @Binding var titleAmbient:String
    @Binding var idJeu:String
    @Binding var colorJeu: String
    
    @State private var selectedColor = Color.red
    @Binding var afficheMenu: Bool
    var color = ["Bleu", "Rouge", "Vert", "Jaune"]
    @State private var selectedCol = 0
    @State private var showingDeleteAlert = false
    @State private var addr = ""
    @State private var newAdrr = ""
    
    var body: some View{
        
        VStack{
            
            VStack{
                
                Button(action: {
                    
                    self.retourMenu()
                }) {
                    
                    Text("Retour Menu")
                        .foregroundColor(.black)
                        .padding(.vertical)
                        .frame(width: UIScreen.main.bounds.width - 50)
                }
                .background(Color.white)
                .cornerRadius(10)
                .padding(.top, 10)
                
                Text("Nom de l'ambiance")
                    .font(.title2)
                    .fontWeight(/*@START_MENU_TOKEN@*/.bold/*@END_MENU_TOKEN@*/)
                    .foregroundColor(Color.white)
                    .padding(.top, 15)
                
                TextField("Titre", text: self.$titleAmbient)
                    .autocapitalization(.none)
                    .padding()
                    .background(RoundedRectangle(cornerRadius: 4).stroke(self.titleAmbient != "" ? Color("Color") : Color.white, lineWidth: 2))
                    .padding(.top, 25)
                
                Text("Nom de la couleur choisie")
                    .font(.title2)
                    .fontWeight(/*@START_MENU_TOKEN@*/.bold/*@END_MENU_TOKEN@*/)
                    .foregroundColor(Color.white)
                    .padding(.top, 15)
                
                Text(self.colorJeu)
                    .autocapitalization(.none)
                    .padding()
                    .background(RoundedRectangle(cornerRadius: 4).stroke(self.colorJeu != "" ? Color("Color") : Color.white, lineWidth: 2))
                    .padding(.top, 25)
                
                /**NavigationView {
                    VStack(spacing: 20) {
                        // 2.
                        Rectangle()
                            .fill(selectedColor)
                            .frame(width: 100, height: 100)
                        // 3.
                        ColorPicker("Couleur choisie", selection: $selectedColor)
                            .padding()
                        
                        Spacer()
                    
                    }.navigationTitle("Choix de la couleur")
                }*/
                
                Text("Nouvelle couleur")
                    .font(.title2)
                    .fontWeight(/*@START_MENU_TOKEN@*/.bold/*@END_MENU_TOKEN@*/)
                    .foregroundColor(Color.white)
                    .padding(.top, 15)
                
                Picker(selection: $selectedCol, label: Text("Color")) {
                                        ForEach(0 ..< color.count) {
                                            Text(self.color[$0])

                                        }
                }
                
            
            
            HStack{
                
                Button(action: {

                    updateItem()
                    
                }) {
                    
                    Text("Mise à jour")
                        .foregroundColor(.black)
                        .padding(.vertical)
                }
                .background(Color.white)
                .cornerRadius(10)
                .padding(.top, 25)
             
                
                Button(action: {

                    self.showingDeleteAlert = true
                }) {
                    
                    Text("Supprimer le jeu")
                        .foregroundColor(.black)
                        .padding(.vertical)
                }
                .alert(isPresented: $showingDeleteAlert) {
                    Alert(title: Text("Supprimer Jeu"), message: Text("Etes-vous sur ?"), primaryButton: .destructive(Text("Supprimer")) {
                            self.deleteJeu()
                    }, secondaryButton: .cancel()
                    )
                }
                .background(Color.white)
                .cornerRadius(10)
                .padding(.top, 25)
             
                Button(action: {
                    /**addr = getWiFiAddress()!
                    if addr != "" {
                        print(addr)
                        addr.removeLast()
                        print(addr)
                        newAdrr = addr + String("1")
                        print(newAdrr)
                    
                    } else {
                       print("No WiFi address")
                    }*/
                    activateJeu()
                }) {
                    
                    Text("Activer ")
                        .foregroundColor(.black)
                        .padding(.vertical)
                }
                .background(Color.white)
                .cornerRadius(10)
                .padding(.top, 25)
                
            }
            
            }
            
        }
    }
    
    
    func deleteJeu(){
        
        let db = Firestore.firestore().collection("ambiance")
        
        db.document(idJeu).delete() { (err) in
            
            if(err != nil){
                
                print((err?.localizedDescription)!)
                return
            }
            print("item deleted")
            
        }
        
        afficheMenu = true
    }
    
    func updateItem(){
        
        let db = Firestore.firestore().collection("ambiance")
        
        db.document(idJeu).updateData(["titre": self.titleAmbient,"couleur":self.color[selectedCol]])
    }
    
    func activateJeu(){
        let url = URL(string: "http://192.168.4.1/color/" + self.colorJeu)!

            let task = URLSession.shared.dataTask(with: url) {(data, response, error) in
                guard let data = data else { return }
                print(String(data: data, encoding: .utf8)!)
            }
            
            task.resume()
    }
    
    
    func retourMenu(){
        
        afficheMenu = true
    }
    
    func getWiFiAddress() -> String? {
        var address : String?

        // Get list of all interfaces on the local machine:
        var ifaddr : UnsafeMutablePointer<ifaddrs>?
        guard getifaddrs(&ifaddr) == 0 else { return nil }
        guard let firstAddr = ifaddr else { return nil }

        // For each interface ...
        for ifptr in sequence(first: firstAddr, next: { $0.pointee.ifa_next }) {
            let interface = ifptr.pointee

            // Check for IPv4 or IPv6 interface:
            let addrFamily = interface.ifa_addr.pointee.sa_family
            //if addrFamily == UInt8(AF_INET) || addrFamily == UInt8(AF_INET6) {  // **ipv6 committed
            if addrFamily == UInt8(AF_INET){

                // Check interface name:
                let name = String(cString: interface.ifa_name)
                if  name == "en0" {

                    // Convert interface address to a human readable string:
                    var hostname = [CChar](repeating: 0, count: Int(NI_MAXHOST))
                    getnameinfo(interface.ifa_addr, socklen_t(interface.ifa_addr.pointee.sa_len),
                                &hostname, socklen_t(hostname.count),
                                nil, socklen_t(0), NI_NUMERICHOST)
                    address = String(cString: hostname)
                }
            }
        }
        freeifaddrs(ifaddr)

        return address
    }
}
