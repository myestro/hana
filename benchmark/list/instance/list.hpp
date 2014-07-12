<%
def includes(n)
    return <<-EOS
        #include <boost/hana/list.hpp>
    EOS
end

def list(xs_t, xs)
  return "boost::hana::list(#{xs.join(', ')})"
end
%>
