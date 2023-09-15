describe 'database' do
    def run_scripts(commands)
        raw_output = nil
        IO.popen("./db", "r+") od |pipe|
            commands.each do |command|
                pipe.puts command
            end 

            pipe.close_write

            raw_output = pipe.gets(nil)

        end
        raw_output.split("\n")


    end
end