function simpleset(block)
setup(block);


function setup(block)   
    block.NumDialogPrms = 1;
    block.NumInputPorts  = 1;
    block.NumOutputPorts  = 1;
    
block.RegBlockMethod('Outputs', @Outputs);  
  
function Outputs(block)   
    
    name = block.DialogPrm(1).Data;
    
    value = block.InputPort(1).Data;
   
    tempV = sprintf( '%g', value );
    
    block.OutputPort(1).Data = value;
    
    temp.p = get_param(gcs,'Blocks');
    temp.i = length(temp.p);
    for i = 1:temp.i
       temp.block = strcat(gcs, '/',temp.p(i));
       temp.see = get_param(temp.block{1},'Name');
       try   
          set_param(temp.block{1} , name, tempV);
       catch
       end     
    end

