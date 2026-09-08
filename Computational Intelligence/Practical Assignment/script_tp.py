from keras.src.saving import load_model

from efficietnet_b0 import train_network_b0,test_network_b0



from meta3PSO import train_PSO, test_network_pso


def run_script():

     model = load_model('b0_model.h5')
     #model,dense,dropout = train_network_b0()
     test_network_b0(model,128,0.2)


     #model,dense,dropout = train_PSO()
     #test_network_pso(model,dense,dropout)


run_script()