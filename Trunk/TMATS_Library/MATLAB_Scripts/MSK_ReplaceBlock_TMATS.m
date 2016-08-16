
function MSK_ReplaceBlock_TMATS(oldblock,newblock)

pos = get_param(oldblock,'Position');
orient = get_param(oldblock,'Orientation');
delete_block(oldblock);
add_block(newblock,oldblock,'Position',pos,'Orientation',orient);

end