% simple secant solver
function [next, last, lasterr] = TMATSC_iterSecant(err, current, lastin, lasterrin, max)
  % determine if this is first pass
  if lastin == -999;
     % first pass, change .05 for initial perturbation if not on soluttion
     if abs(err)>.000000001
        next = current*1.05;
     else
         % first pass worked, dont change value           
         next = current;
     end
  else
      % not first pass, determine the next guess value    
      next = current - err * ( current - lastin )/(err - lasterrin );
      % check to see if we violate the maximum value
      if abs(( next - current )/current) > max
          next = current + .1*current*sign(next -current );
      end
  end
  %return these values for use on next pass as previous results
  last = current;
  lasterr = err;

        

